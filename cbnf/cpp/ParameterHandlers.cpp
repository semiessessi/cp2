// Copyright (c) 2014, 2017 Cranium Software

#include "Parameters.h"

#include <cstring>
#include <string>

int giVerbosity = 0;
std::string gxOutputPath;

bool gbCPPOutput = false;
bool gbVSIXOutput = false;

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

int CPPHandler( const char* const szParameter )
{
	gbCPPOutput = true;

	return 0;
}

int VSIXHandler( const char* const szParameter )
{
	gbVSIXOutput = true;

	return 0;
}
