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

void WriteBinaryFile( const char* const szPath, const void* const pData, const size_t uLength )
{
	FILE* const pxFile = fopen( szPath, "wb" );
	if ( pxFile == nullptr )
	{
		return;
	}

	fwrite( pData, 1, uLength, pxFile );
}

void WriteTextFile( const char* const szPath, const char* const szText )
{
	WriteBinaryFile( szPath, szText, strlen( szText ) );
}

}
