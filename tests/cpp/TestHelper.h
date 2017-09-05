#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "../../common/cpp/Report.h"

#include <functional>

namespace CP2
{
namespace Tests
{

extern int giTestsPassed;
extern int giTestsFailed;

static inline bool Expect(
	const bool bCondition,
	const std::function< void( void ) > & xCallback = [](){} )
{
	if( !bCondition )
	{
		xCallback();
		++giTestsFailed;
		return false;
	}
	else
	{
		Message( "Test passed" );
	}

	++giTestsPassed;
	return true;
}

static inline bool ExpectClean( const char* const szName )
{
	return Expect( ( GetErrorCount() == 0 ) && ( GetWarningCount() == 0 ), [ = ]()
	{
		Message( "error: Failed test: \"%s\", expected no errors or warnings", szName );
	} );
}

static inline bool ExpectSingleError( const char* const szName, const int iCode )
{
	std::vector< ReportMessage > axReports( GetAllMessages() );
	return Expect( ( axReports.size() == 1 )
		&& ( axReports[ 0 ].iCode == iCode )
		&& ( axReports[ 0 ].bError == true )
		&& ( axReports[ 0 ].bInternal == false ), [ = ]()
	{
		Message( "error: Failed test: \"%s\", expected error %d", szName, iCode );
	} );
}

static inline bool ExpectSingleInternalError( const char* const szName, const int iCode )
{
	std::vector< ReportMessage > axReports( GetAllMessages() );
	return Expect( ( axReports.size() == 1 )
		&& ( axReports[ 0 ].iCode == iCode )
		&& ( axReports[ 0 ].bError == true )
		&& ( axReports[ 0 ].bInternal == true ), [ = ]()
	{
		Message( "error: Failed test: \"%s\", expected internal error %d", szName, iCode );
	} );
}

static inline bool ExpectSingleWarning( const char* const szName, const int iCode )
{
	std::vector< ReportMessage > axReports( GetAllMessages() );
	return Expect( ( axReports.size() == 1 )
		&& ( axReports[ 0 ].iCode == iCode )
		&& ( axReports[ 0 ].bWarning == true )
		&& ( axReports[ 0 ].bInternal == false ), [ = ]()
	{
		Message( "error: Failed test: \"%s\", expected warning %d", szName, iCode );
	} );
}

}
}

#endif
