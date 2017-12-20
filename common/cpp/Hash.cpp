#include "Hash.h"

#include <iomanip>
#include <sstream>

namespace CP2
{

unsigned int JenkinsOATHash( const std::string_view& xInput )
{
	unsigned int uReturn = 0;
	for( int i = 0; i < xInput.length(); ++i )
	{
		uReturn += xInput[ i ];
		uReturn += uReturn << 10;
		uReturn ^= uReturn >> 6;
	}

	uReturn += uReturn << 3;
	uReturn ^= uReturn >> 11;
	uReturn += uReturn << 15;
	
	return uReturn;
}

unsigned int BernsteinHash( const std::string_view& xInput )
{
	unsigned int uReturn = 0;
	for ( int i = 0; i < xInput.length(); ++i )
	{
		uReturn = ( uReturn * 33 ) + xInput[ i ];
	}

	return uReturn;
}

unsigned int ModifiedBernsteinHash( const std::string_view& xInput )
{
	unsigned int uReturn = 0;
	for ( int i = 0; i < xInput.length(); ++i )
	{
		uReturn = ( uReturn * 33 ) ^ xInput[ i ];
	}

	return uReturn;
}

unsigned int SAXHash( const std::string_view& xInput )
{
	unsigned int uReturn = 0;
	for ( int i = 0; i < xInput.length(); ++i )
	{
		uReturn ^= ( uReturn << 5 ) + ( uReturn >> 2 ) + xInput[ i ];
	}

	return uReturn;
}

unsigned int FNVHash( const std::string_view& xInput )
{
	unsigned int uReturn = 2166136261;
	for ( int i = 0; i < xInput.length(); ++i )
	{
		uReturn = ( uReturn * 16777619 ) ^ xInput[ i ];
	}

	return uReturn;
}

Hash GetHash( const std::string_view& xInput )
{
	const Hash xReturnValue =
	{
		{
			JenkinsOATHash( xInput ),
			BernsteinHash( xInput ) ^ ModifiedBernsteinHash( xInput ),
			SAXHash( xInput ),
			FNVHash( xInput ),
		}
	};

	return xReturnValue;
}

std::string HashToGUID( const Hash& xHash )
{
	std::stringstream xStream;
	// e.g. "FAE04EC0-301F-11D3-BF4B-00C04F79EFBC"
	xStream << std::setfill( '0' ) << std::setw( 8 ) << std::hex << xHash.auData[ 0 ];
	xStream << "-";
	xStream << std::setfill( '0' ) << std::setw( 4 ) << std::hex << ( xHash.auData[ 1 ] & 0xffff );
	xStream << "-";
	xStream << std::setfill( '0' ) << std::setw( 4 ) << std::hex << ( ( xHash.auData[ 1 ] >> 16 ) & 0xffff );
	xStream << "-";
	xStream << std::setfill( '0' ) << std::setw( 4 ) << std::hex << ( xHash.auData[ 2 ] & 0xffff );
	xStream << "-";
	xStream << std::setfill( '0' ) << std::setw( 4 ) << std::hex << ( ( xHash.auData[ 2 ] >> 16 ) & 0xffff );
	xStream << std::setfill( '0' ) << std::setw( 8 ) << std::hex << xHash.auData[ 3 ];

	return xStream.str();
}

}
