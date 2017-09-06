#include "Lexer.h"

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
	if( mszMarkerStart == nullptr )
	{
		return 0;
	}

	if( strncmp( szCursor, mszMarkerStart, strlen( mszMarkerStart ) ) != 0 )
	{
		return 0;
	}

	const char* szEnd = szCursor;
	if( mszMarkerEnd != nullptr )
	{
		// find the end.
		szEnd = strstr( szCursor, mszMarkerEnd );
		if( szEnd == nullptr )
		{
			// consider the whole remainder to be comment, but warn the user
			CP2::Warning( 2501, szFilename, iLine, iColumn, "Unterminated comment - found end of file before %s", mszMarkerEnd );
			return static_cast< int >( strlen( szCursor ) );
		}

		szEnd += strlen( mszMarkerEnd );
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

static inline const char* FindNewCursor( const char* const szStartCursor )
{
	const char* szNewCursor = szStartCursor;
	while( *szNewCursor
		&& ( *szNewCursor != ' ' )
		&& ( *szNewCursor != '\t' )
		&& ( *szNewCursor != '\r' )
		&& ( *szNewCursor != '\n' ) )
	{
		// skip quoted stuff...
		++szNewCursor;
		if( *( szNewCursor - 1 ) == '"' )
		{
			while( *szNewCursor &&
				!( ( *( szNewCursor - 1 ) != '\\' )
					&& ( *( szNewCursor ) == '"' ) ) )
			{
				++szNewCursor;
			}
			++szNewCursor;
			return szNewCursor;
		}
	}

	return szNewCursor;
}

static inline void UpdateLongest( const int i,
	const std::string& xValue, std::string& xLongestValue,
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
		if( ( iCachedStringLength != 0 )
			&& ( iCachedStringLength > iLongestLength ) )
		{
			const std::string xValue(
				szCursor, szCursor + iCachedStringLength );
			if( axStrings[ i ] == xValue )
			{
				UpdateLongest( static_cast< int >( i ),
					xValue, xLongestValue, iLongestLength, iLongestRule );
			}

			// otherwise, we know we didn't match, don't waste time on regexes
		}
		else if( std::regex_match( szCursor, szNewCursor, xMatch, axRegexes[ i ] ) )
		{
			UpdateLongest( static_cast< int >( i ),
				xMatch.str( 0 ), xLongestValue, iLongestLength, iLongestRule );
		}
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
	const std::vector< Comment >& axComments, const std::vector< Rule >& axRules,
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
			const char* szNewCursor = FindNewCursor( szCursor );
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

static inline bool IsRuleNotRegex( const std::string& xRule )
{
	bool bSafe = true;
	for( size_t i = 0; i < xRule.length(); ++i )
	{
		bSafe &= ( xRule[  i ] != '\\' )
			&& ( xRule[ i ] != '.' )
			&& ( xRule[ i ] != '(' )
			&& ( xRule[ i ] != '[' )
			&& ( xRule[ i ] != '$' )
			&& ( xRule[ i ] != ':' );

		if( i > 0 )
		{
			bSafe &= ( xRule[ i ] != ')' )
				&& ( xRule[ i ] != ']' )
				&& ( xRule[ i ] != '-' )
				&& ( xRule[ i ] != ':' )
				&& ( xRule[ i ] != '+' )
				&& ( xRule[ i ] != '*' )
				&& ( xRule[ i ] != '^' )
				&& ( xRule[ i ] != '?' );
		}
	}

	return bSafe;
}

static inline std::vector< std::basic_regex< char > > BuildRegexCache(
	const std::vector< Rule >& axRules )
{
	std::vector< std::basic_regex< char > > axRegexes;
	for( size_t i = 0; i < axRules.size(); ++i )
	{
		axRegexes.push_back( std::basic_regex< char >(
			axRules[ i ].GetExpression().c_str() ) );
	}

	return axRegexes;
}

static inline std::vector< std::string > BuildStringCache(
	const std::vector< Rule >& axRules )
{
	std::vector< std::string > axStrings;
	for( size_t i = 0; i < axRules.size(); ++i )
	{
		if( IsRuleNotRegex( axRules[ i ].GetExpression() ) )
		{
			axStrings.push_back( axRules[ i ].GetExpression() );
		}
		else
		{
			axStrings.push_back( std::string() );
		}
	}

	return axStrings;
}

std::vector< Token > Lex( const char* const szFilename, const char* const szSourceCode,
	const std::vector< Rule >& axRules, const std::vector< Comment >& axComments )
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
		HandleNextCharacter( axComments, axRules,
			axTokens, szCursor,
			xMatch, axRegexes, axStrings,
			szFilename, iLine, iColumn );
	}

	return axTokens;
}

std::vector< Token > Lex( const char* const szFilename,
	const std::vector< Rule >& axRules, const std::vector< Comment >& axComments )
{
	std::vector< Token > axTokens;

	FILE* const pxFile = fopen( szFilename, "rb" );
	if( pxFile == nullptr )
	{
		Error( 2001, szFilename, 0, 0, "Unable to open file: %s", szFilename );
		return axTokens;
	}

	fseek( pxFile, 0, SEEK_END );
	const int iLength = ftell( pxFile );
	rewind( pxFile );
	char* const pcData = new char[ iLength + 1 ];
	pcData[ iLength ] = 0;
	fread( pcData, 1, iLength, pxFile );
	fclose( pxFile );

	axTokens = Lex( szFilename, pcData, axRules, axComments );

	delete[] pcData;
	return axTokens;
}

}
}
