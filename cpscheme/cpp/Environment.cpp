// Copyright (c) 2017 Cranium Software

#include "Environment.h"

#include "Evaluate.h"

namespace CP2
{
namespace Scheme
{

Environment Environment::sxGlobalEnvironment = Environment::NewDefaultEnvironment();

Environment::~Environment()
{
	// delete variables
	for( auto xEntry : mxVariables )
	{
		delete xEntry.second;
	}
}

Environment::Environment( const Environment& xOther )
{
	for( auto xEntry : xOther.mxVariables )
	{
		mxVariables[ xEntry.first ] = new EvaluationResult;
		*( mxVariables[ xEntry.first ] ) = *( xEntry.second );
	}
}

Environment Environment::NewDefaultEnvironment()
{
	Environment xEnvironment;

	return xEnvironment;
}

bool Environment::VariableIsDefined( const char* const szName ) const
{
	return mxVariables.find( szName ) != mxVariables.end();
}

EvaluationResult& Environment::GetVariable( const char* const szName )
{
	if( mxVariables.find( szName ) == mxVariables.end() )
	{
		mxVariables[ szName ] = new EvaluationResult;
	}

	return *( mxVariables[ szName ] );
}

}
}
