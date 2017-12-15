// Copyright (c) 2017 Cranium Software

#include "FileSystem.h"

#ifdef _WIN32
#include <direct.h>
#else

#endif

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

}
