#ifndef ESCAPING_H
#define ESCAPING_H

#include <string>

namespace CP2
{

static inline void RegexEscapeCharacter(
	const char cCharaceter, size_t& uOffset, std::string& xCopy )
{
	uOffset = xCopy.find( cCharaceter, 0 );
	while( uOffset != std::string::npos )
	{
		xCopy.insert( uOffset, "\\" );
		uOffset += 2;
		uOffset = xCopy.find( cCharaceter, uOffset );
	}
}
static inline std::string RegexEscape( const std::string& xSource )
{
	std::string xCopy = xSource;

	size_t uOffset = 0;
	// always do \ first to avoid escaping explosion
	RegexEscapeCharacter( '\\', uOffset, xCopy );

	RegexEscapeCharacter( '+', uOffset, xCopy );
	RegexEscapeCharacter( '-', uOffset, xCopy );
	RegexEscapeCharacter( '*', uOffset, xCopy );
	RegexEscapeCharacter( '?', uOffset, xCopy );
	RegexEscapeCharacter( '^', uOffset, xCopy );
	RegexEscapeCharacter( ':', uOffset, xCopy );
	RegexEscapeCharacter( '.', uOffset, xCopy );

	return xCopy;
}

static inline std::string SimpleUnescape( const std::string& xSource )
{
	std::string xCopy = xSource;

	size_t uOffset = xCopy.find( '\\', 0 );
	while( uOffset != std::string::npos )
	{
		xCopy.erase( uOffset );
		++uOffset;
		uOffset = xCopy.find( '\\', uOffset );
	}

	return xCopy;
}

}

#endif
