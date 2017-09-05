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

static inline bool MatchRules(
	const std::vector< Rule >& axRules,
	std::vector< Token >& axTokens, const char*& szCursor, const char* const szNewCursor,
	std::cmatch& xMatch, const std::vector< std::basic_regex< char > >& axRegexes,
	const char* const szFilename, int& iLine, int& iColumn )
{
	// check if we match a rule...
	std::string xLongestValue = "";
	int iLongestRule = -1;
	for( size_t i = 0; i < axRules.size(); ++i )
	{
		if( std::regex_match( szCursor, szNewCursor, xMatch, axRegexes[ i ] ) )
		{
			const std::string xValue = xMatch.str( 0 );
			if( xValue.length() > xLongestValue.length() )
			{
				xLongestValue = xValue;
				iLongestRule = static_cast< int >( i );
			}
		}
		else if( std::regex_search( szCursor, szNewCursor, xMatch, axRegexes[ i ] ) )
		{
			// make sure the match is at the start of the string...
			if( xMatch.prefix().length() == 0 )
			{
				const std::string xValue = xMatch.str( 0 );
				if( xValue.length() > xLongestValue.length() )
				{
					xLongestValue = xValue;
					iLongestRule = static_cast< int >( i );
				}
			}
		}
	}

	if( iLongestRule >= 0 )
	{
		axTokens.push_back( Token(
			axRules[ iLongestRule ].GetBaseToken(),
			szFilename, iLine, iColumn, xLongestValue ) );
		szCursor += xLongestValue.length();
		iColumn += static_cast< int >( xLongestValue.length() );
		// SE - TODO: handle new lines in matches.
		return true;
	}

	return false;
}

static inline void HandleNextCharacter(
	const std::vector< Comment >& axComments, const std::vector< Rule >& axRules,
	std::vector< Token >& axTokens, const char*& szCursor,
	std::cmatch& xMatch, const std::vector< std::basic_regex< char > >& axRegexes,
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
				xMatch, axRegexes, szFilename, iLine, iColumn ) )
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
	std::vector< std::basic_regex< char > > axRegexes;
	for( size_t i = 0; i < axRules.size(); ++i )
	{
		axRegexes.push_back( std::basic_regex< char >( axRules[ i ].GetExpression().c_str() ) );
	}

	while( *szCursor )
	{
		HandleNextCharacter( axComments, axRules,
			axTokens, szCursor,
			xMatch, axRegexes,
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
