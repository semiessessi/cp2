// Copyright (c) 2017 Cranium Software

#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>

namespace CP2
{

struct ReportMessage
{
	std::string szMessage;
	int iCode;
	bool bInternal;
	bool bError;
	bool bWarning;
};

void ResetMessageReports();
int GetErrorCount();
int GetWarningCount();
std::vector< ReportMessage > GetAllMessages();

void UntrackedMessage( const char* const szString, ... );
void Message( const char* const szString, ... );
void Report( const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... );
void Warning( const int iWarningNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... );
void Error( const int iErrorNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... );
void InternalError( const int iErrorNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... );

}

#endif
