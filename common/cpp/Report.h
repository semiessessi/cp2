// Copyright (c) 2017 Cranium Software

#ifndef REPORT_H
#define REPORT_H

namespace CP2
{

void ResetCounters();
int GetErrorCount();
int GetWarningCount();
void Message( const char* const szString, ... );
void Report( const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... );
void Warning( const int iWarningNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... );
void Error( const int iErrorNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... );
void InternalError( const int iErrorNumber, const char* const szFilename, const int iLine, const int iColumn, const char* const szString, ... );

}

#endif
