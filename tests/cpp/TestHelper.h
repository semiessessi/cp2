#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "../../common/cpp/Report.h"

namespace CP2
{
namespace Tests
{

static inline bool ExpectClean( const char* const szName )
{
	if( ( GetErrorCount() != 0 ) || ( GetWarningCount() != 0 ) )
	{
		Message( "error: Failed test: \"%s\", expected no errors or warnings", szName );
		return false;
	}

	return true;
}

static inline bool ExpectSingleError( const char* const szName, const int iCode )
{
	std::vector< ReportMessage > axReports( GetAllMessages() );
	if( ( axReports.size() != 1 )
		|| ( axReports[ 0 ].iCode != iCode )
		|| ( axReports[ 0 ].bError != true ) )
	{
		Message( "error: Failed test: \"%s\", expected error %d", szName, iCode );
		return false;
	}

	return true;
}

static inline bool ExpectSingleInternalError( const char* const szName, const int iCode )
{
	std::vector< ReportMessage > axReports( GetAllMessages() );
	if( ( axReports.size() != 1 )
		|| ( axReports[ 0 ].iCode != iCode )
		|| ( axReports[ 0 ].bInternal != true )
		|| ( axReports[ 0 ].bError != true ) )
	{
		Message( "error: Failed test: \"%s\", expected internal error %d", szName, iCode );
		return false;
	}

	return true;
}

static inline bool ExpectSingleWarning( const char* const szName, const int iCode )
{
	std::vector< ReportMessage > axReports( GetAllMessages() );
	if( ( axReports.size() != 1 )
		|| ( axReports[ 0 ].iCode != iCode )
		|| ( axReports[ 0 ].bWarning != true ) )
	{
		Message( "error: Failed test: \"%s\", expected warning %d", szName, iCode );
		return false;
	}

	return true;
}

}
}

#endif
