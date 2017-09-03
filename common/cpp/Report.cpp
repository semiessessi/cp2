// Copyright (c) 2017 Cranium Software

#include "Report.h"

#include <cstdarg>
#include <cstdio>

namespace CP2
{

int giErrorCount = 0;
int giWarningCount = 0;

void ResetCounters()
{
	giErrorCount = 0;
	giWarningCount = 0;
}

int GetErrorCount()
{
	return giErrorCount;
}

int GetWarningCount()
{
	return giWarningCount;
}

void Message( const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	puts( szBuffer );
}

void Report( const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	Message( "%s(%d,%d): %s", szFilename, iLine, iColumn, szBuffer );
}

void Warning( const int iWarningNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	++giWarningCount;
	Report( szFilename, iLine, iColumn, "warning %.4d: %s", iWarningNumber, szBuffer );
}

void Error( const int iErrorNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	++giErrorCount;
	Report( szFilename, iLine, iColumn, "error %.4d: %s", iErrorNumber, szBuffer );
}

void InternalError( const int iErrorNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	++giErrorCount;
	Report( szFilename, iLine, iColumn, "internal error %.4d: %s", iErrorNumber, szBuffer );
}

}
