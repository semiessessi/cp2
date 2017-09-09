// Copyright (c) 2017 Cranium Software

#include "Profiling.h"

#include "Report.h"

#include <ctime>
#include <vector>

namespace CP2
{

static std::vector< double > gaxStartTimes;

double GetTimeInSeconds()
{
	return static_cast< double >( clock() )
		/ static_cast< double >( CLOCKS_PER_SEC );
}

void StartProfiling( const char* const /*szName*/ )
{
	gaxStartTimes.push_back( GetTimeInSeconds() );
}

void EndProfiling( const char* const szName )
{
	const double dTime = GetTimeInSeconds() - gaxStartTimes.back();
	gaxStartTimes.pop_back();

	Message( "Profiling: %s took %.3f seconds", szName, dTime );
}

}
