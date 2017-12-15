// Copyright (c) 2017 Cranium Software

#include "FileSystem.h"

#ifdef _WIN32
#include <direct.h>
#else

#endif

#include <cstdio>
#include <cstring>

namespace CP2
{

void EnsurePath( const char* const szPath )
{
#ifdef _WIN32
	_mkdir( szPath );
	// SE - TODO: some error handling...
#else

#endif
}

void WriteTextFile( const char* const szPath, const char* const szText )
{
	FILE* const pxFile = fopen( szPath, "wb" );
	if( pxFile == nullptr )
	{
		return;
	}

	const size_t uLength = strlen( szText );
	fwrite( szText, 1, uLength, pxFile );
}

}
