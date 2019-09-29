#include "Lexer.h"

#include "../../common/cpp/Escaping.h"
#include "../../common/cpp/Report.h"

#include <cstdio>
#include <cstring>
#include <regex>

namespace CP2
{
namespace Lexer
{

int Comment::GetLength( const char* const szCursor, const char* const szFilename, const int iLine, const int iColumn ) const
{
	if( mszMarkerStart.empty() )
	{
		return 0;
	}

	if( strncmp( szCursor, mszMarkerStart.c_str(), strlen( mszMarkerStart.c_str() ) ) != 0 )
	{
		return 0;
	}

	const char* szEnd = szCursor;
	if( !mszMarkerEnd.empty() )
	{
		// find the end.
		szEnd = strstr( szCursor, mszMarkerEnd.c_str() );
		if( szEnd == nullptr )
		{
			// consider the whole remainder to be comment, but warn the user
			CP2::Warning( 2501, szFilename, iLine, iColumn, "Unterminated comment - found end of file before %s", mszMarkerEnd.c_str() );
			return static_cast< int >( strlen( szCursor ) );
		}

		szEnd += strlen( mszMarkerEnd.c_str() );
		return static_cast< int >( szEnd - szCursor );
	}
	//else
	{
		// find the next newline
		szEnd = strstr( szCursor, "\n" );
		if( szEnd == nullptr )
		{
			// consider the whole remainder to be comment.
			return static_cast< int >( strlen( szCursor ) );
		}

		return static_cast< int >( szEnd - szCursor );
	}
}

static inline void HandleCounts( const char* const szCursor, int& iLine, int& iColumn )
{
	switch( *szCursor )
	{
		case '\n':
		{
			++iLine;
			iColumn = 1;
			break;
		}
		case '\t':
		{
			iColumn += kiTabSpaces;
			break;
		}
		case '\r':
		{
			break;
		}
		default:
		{
			++iColumn;
			break;
		}
	}
}

static inline bool HandleComments(
	const std::vector< Comment >& axComments, const char*& szCursor,
	const char* const szFilename, int& iLine, int& iColumn )
{
	for( size_t i = 0; i < axComments.size(); ++i )
	{
		const int iCommentLength = axComments[ i ].GetLength(
			szCursor, szFilename, iLine, iColumn );
		if( iCommentLength > 0 )
		{
			// update the line and column numbers...
			for( int i = 0; i < iCommentLength; ++i )
			{
				HandleCounts( szCursor, iLine, iColumn );
				++szCursor;
			}
			
			return true;
		}
	}

	return false;
}

// iQuote is set to a positive or zero if it matches a quoted string
static inline const char* FindNewCursor(
	const char* const szStartCursor, const std::vector< Quote >& axQuotes,
	const char* const szFilename, int& iLine, int& iColumn, int& iQuote )
{
	const char* szNewCursor = szStartCursor;
	for( int i = 0; i < static_cast< int >( axQuotes.size() ); ++i )
	{
		const Quote& xQuote = axQuotes[ i ];
		// are we at the start of this quote? then grab it and return it... 
        const int iCompareResult = strncmp(
            szNewCursor, xQuote.GetStart(), xQuote.GetStartLength() );
		if( iCompareResult == 0 )
		{
			iQuote = i;
			// advance past the quote start
			szNewCursor += xQuote.GetStartLength();
			while( *szNewCursor )
			{
				if( ( xQuote.GetEscapeLength() > 0 ) 
				  && ( strncmp(
					szNewCursor, xQuote.GetEscape(), xQuote.GetEscapeLength() ) == 0 ) )
				{
					// skip the escape sequence, and its following character
					szNewCursor += xQuote.GetEscapeLength();
				}
				else if( strncmp(
					szNewCursor, xQuote.GetEnd(), xQuote.GetEndLength() ) == 0 )
				{
					szNewCursor += xQuote.GetEndLength();
					return szNewCursor;
				}

				++szNewCursor;
			}

			CP2::Error( 2003, szFilename, iLine, iColumn, "Unterminated quote - found end of file before %s", xQuote.GetEnd() );
			return szNewCursor;
		}
	}

	while( *szNewCursor
		&& ( *szNewCursor != ' ' )
		&& ( *szNewCursor != '\t' )
		&& ( *szNewCursor != '\r' )
		&& ( *szNewCursor != '\n' ) )
	{
		// skip until we hit whitespace or the end of the file...
		++szNewCursor;
	}

	return szNewCursor;
}

static inline void UpdateLongest( const int i,
	const std::string_view& xValue, std::string& xLongestValue,
	int& iLongestLength, int& iLongestRule )
{
	const int iNewLength = static_cast< int >( xValue.length() );
	if( iNewLength <= iLongestLength )
	{
		return;
	}

	xLongestValue = xValue;
	iLongestLength = iNewLength;
	iLongestRule = i;
}

static inline bool MatchRules(
	const std::vector< Rule >& axRules,
	std::vector< Token >& axTokens, const char*& szCursor, const char* const szNewCursor,
	std::cmatch& xMatch, const std::vector< std::basic_regex< char > >& axRegexes,
	const std::vector< std::string >& axStrings,
	const char* const szFilename, int& iLine, int& iColumn )
{
	// check if we match a rule...
	std::string xLongestValue = "";
	int iLongestRule = -1;
	int iLongestLength = -1;
	for( size_t i = 0; i < axRules.size(); ++i )
	{
		const int iCachedStringLength =
			static_cast< int >( axStrings[ i ].length() );
		// if the rule is a string, and longer than the current match
		// then check it
		if( ( iCachedStringLength != 0 )
			&& ( iCachedStringLength > iLongestLength ) )
		{
			const std::string_view xValue(
				szCursor, iCachedStringLength );
			if( axStrings[ i ] == xValue )
			{
				UpdateLongest( static_cast< int >( i ),
					xValue, xLongestValue, iLongestLength, iLongestRule );
			}

			// otherwise, we know we didn't match, don't waste time on regexes
		}
		// otherwise, if we exactly match, based on our guess of where the token ends
		// then we update the current match
		else if( std::regex_match( szCursor, szNewCursor, xMatch, axRegexes[ i ] ) )
		{
			UpdateLongest( static_cast< int >( i ),
				xMatch.str( 0 ), xLongestValue, iLongestLength, iLongestRule );
		}
		// otherwise we try and match anywhere inside the string we are inspecting
		else if( std::regex_search( szCursor, szNewCursor, xMatch, axRegexes[ i ] ) )
		{
			// make sure the match is at the start of the string...
			if( xMatch.prefix().length() == 0 )
			{
				UpdateLongest( static_cast< int >( i ),
					xMatch.str( 0 ), xLongestValue, iLongestLength, iLongestRule );
			}
		}
	}

	if( iLongestRule >= 0 )
	{
		axTokens.push_back( Token(
			axRules[ iLongestRule ].GetBaseToken(),
			szFilename, iLine, iColumn, xLongestValue ) );
		szCursor += iLongestLength;
		// SE - TODO: handle new lines in matches.
		iColumn += iLongestLength;
		return true;
	}

	return false;
}

static inline void HandleNextCharacter(
	const std::vector< Comment >& axComments,
	const std::vector< Rule >& axRules,
	const std::vector< Quote >& axQuotes,
	std::vector< Token >& axTokens, const char*& szCursor,
	std::cmatch& xMatch, const std::vector< std::basic_regex< char > >& axRegexes,
	const std::vector< std::string >& axStrings,
	const char* const szFilename, int& iLine, int& iColumn )
{
	switch( *szCursor )
	{
		case ' ':
		case '\n':
		case '\r':
		case '\t':
		{
			HandleCounts( szCursor, iLine, iColumn );
			++szCursor;
			break;
		}
		default:
		{
			// check if we match a comment...
			if( HandleComments( axComments, szCursor, szFilename, iLine, iColumn ) )
			{
				break;
			}

			// work out where the end of the next token is by whitespace, ignoring quotes...
			int iQuote = -1;
			const char* szNewCursor = FindNewCursor( szCursor, axQuotes, szFilename, iLine, iColumn, iQuote );
			if( iQuote >= 0 )
			{
				// SE - TODO: does the id matter?
				const Quote& xQuote = axQuotes[ iQuote ];
				const Token xBaseToken( xQuote.GetName(), 1337, true );
				axTokens.push_back( Token(
					xBaseToken,
					szFilename, iLine, iColumn, std::string( szCursor, szNewCursor - szCursor ) ) );
				szCursor = szNewCursor;
				// SE - TODO: handle new lines in matches.
				iColumn += static_cast< int >( szNewCursor - szCursor );
				break;
			}

			if( MatchRules(
				axRules, axTokens, szCursor, szNewCursor,
				xMatch, axRegexes, axStrings,
				szFilename, iLine, iColumn ) )
			{
				break;
			}

			Error( 2002, szFilename, iLine, iColumn, "Failed to tokenise here: %s",
				std::string( szCursor, szNewCursor ).c_str() );

			// carry on anyway ... 
			szCursor = szNewCursor;
			break;
		}
	}
}

static inline std::vector< std::basic_regex< char > > BuildRegexCache(
	const std::vector< Rule >& axRules )
{
	std::vector< std::basic_regex< char > > axRegexes;
	for( size_t i = 0; i < axRules.size(); ++i )
	{
		if( !axRules[ i ].GetBaseToken().IsValued() )
		{
			// this regex should never match.
			axRegexes.push_back( std::basic_regex< char >( "(?!)" ) );
		}
		else
		{
			axRegexes.push_back( std::basic_regex< char >(
				axRules[ i ].GetExpression().c_str(),
				std::regex_constants::optimize
					| std::regex_constants::nosubs ) );
		}
	}

	return axRegexes;
}

static inline std::vector< std::string > BuildStringCache(
	const std::vector< Rule >& axRules )
{
	std::vector< std::string > axStrings;
	for( size_t i = 0; i < axRules.size(); ++i )
	{
		if( !axRules[ i ].GetBaseToken().IsValued() )
		{
			axStrings.push_back(
				SimpleUnescape( axRules[ i ].GetExpression() ) );
		}
		else
		{
			axStrings.push_back( std::string() );
		}
	}

	return axStrings;
}

std::vector< Token > Lex( const char* const szFilename, const char* const szSourceCode,
	const std::vector< Rule >& axRules,
	const std::vector< Comment >& axComments,
	const std::vector< Quote >& axQuotes )
{
	std::vector< Token > axTokens;

	if( axRules.size() == 0 )
	{
		InternalError( 2401, szFilename, 0, 0, "Empty ruleset provided for lexical analysis" );
		return axTokens;
	}

	int iColumn = 1;
	int iLine = 1;

	const char* szCursor = szSourceCode;
	
	// reuse and cache stuff for speed...
	std::cmatch xMatch;
	std::vector< std::basic_regex< char > > axRegexes( BuildRegexCache( axRules ) );
	std::vector< std::string > axStrings( BuildStringCache( axRules ) );
	while( *szCursor )
	{
		HandleNextCharacter(
			axComments, axRules, axQuotes,
			axTokens, szCursor,
			xMatch, axRegexes, axStrings,
			szFilename, iLine, iColumn );
	}

	return axTokens;
}

std::vector< Token > Lex( const char* const szFilename,
	const std::vector< Rule >& axRules,
	const std::vector< Comment >& axComments,
	const std::vector< Quote >& axQuotes )
{
	std::vector< Token > axTokens;

	FILE* const pxFile = fopen( szFilename, "rb" );
	if( pxFile == nullptr )
	{
		Error( 2001, szFilename, 0, 0, "Unable to open file: %s", szFilename );
		return axTokens;
	}

	fseek( pxFile, 0, SEEK_END );
	const int iLength = static_cast< int >( ftell( pxFile ) );
	rewind( pxFile );
	char* const pcData = new char[ iLength + 1 ];
	pcData[ iLength ] = 0;
	fread( pcData, 1, iLength, pxFile );
	fclose( pxFile );

	axTokens = Lex( szFilename, pcData, axRules, axComments, axQuotes );

	delete[] pcData;
	return axTokens;
}

}
}
