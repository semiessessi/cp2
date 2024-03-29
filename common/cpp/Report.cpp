// Copyright (c) 2017 Cranium Software

#include "Report.h"

#include <cstdarg>
#include <cstdio>

namespace CP2
{

int giErrorCount = 0;
int giWarningCount = 0;

std::vector< ReportMessage > gaxReportMessages;

void LastMessageWasWarning()
{
	++giWarningCount;
	gaxReportMessages.back().bWarning = true;
}

void LastMessageWasError()
{
	++giErrorCount;
	gaxReportMessages.back().bError = true;
}

void LastMessageWasInternalError()
{
	++giErrorCount;
	gaxReportMessages.back().bError = true;
	gaxReportMessages.back().bInternal = true;
}

void SetLastMessageCode( const int iCode )
{
	gaxReportMessages.back().iCode = iCode;
}

void ResetMessageReports()
{
	giErrorCount = 0;
	giWarningCount = 0;
	gaxReportMessages.clear();
}

int GetErrorCount()
{
	return giErrorCount;
}

int GetWarningCount()
{
	return giWarningCount;
}

std::vector< ReportMessage > GetAllMessages()
{
	return gaxReportMessages;
}


void UntrackedMessage( const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	puts( szBuffer );
}

void Message( const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	puts( szBuffer );

	ReportMessage xMessage =
	{
		szBuffer,
		0,
		false,
		false,
		false,
	};

	gaxReportMessages.push_back( xMessage );
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

	Report( szFilename, iLine, iColumn, "warning %.4d: %s", iWarningNumber, szBuffer );

	LastMessageWasWarning();
	SetLastMessageCode( iWarningNumber );
}

void Error( const int iErrorNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	Report( szFilename, iLine, iColumn, "error %.4d: %s", iErrorNumber, szBuffer );

	LastMessageWasError();
	SetLastMessageCode( iErrorNumber );
}

void InternalError( const int iErrorNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... )
{
	char szBuffer[ 256 ];
	va_list xArguments;
	va_start( xArguments, szString );
	vsnprintf( szBuffer, 256, szString, xArguments );
	va_end( xArguments );

	Report( szFilename, iLine, iColumn, "internal error %.4d: %s", iErrorNumber, szBuffer );

	LastMessageWasInternalError();
	SetLastMessageCode( iErrorNumber );
}

}
