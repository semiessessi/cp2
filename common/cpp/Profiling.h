// Copyright (c) 2017 Cranium Software

#ifndef PROFILING_H
#define PROFILING_H

namespace CP2
{

double GetTimeInSeconds();
void StartProfiling( const char* const szName );
void EndProfiling( const char* const szName );

}

#endif
