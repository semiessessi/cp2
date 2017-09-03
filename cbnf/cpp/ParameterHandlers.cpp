// Copyright (c) 2014, 2017 Cranium Software

#include "Parameters.h"

#include <cstring>
#include <string>

int giVerbosity = 0;
std::string gxOutputPath;

int VerbosityHandler( const char* const szParameter )
{
	giVerbosity = atoi( szParameter );
    
    return 0;
}

int OutputHandler( const char* const szParameter )
{
	gxOutputPath = szParameter;

	return 0;
}
