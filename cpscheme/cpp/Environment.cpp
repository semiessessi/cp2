// Copyright (c) 2017 Cranium Software

#include "Environment.h"

#include "Evaluate.h"
#include "StandardLibrary.h"

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

	// constants
	xEnvironment.GetVariable( "e" ) = EvaluationResult( 2.718281828f );
	xEnvironment.GetVariable( "pi" ) = EvaluationResult( 3.141592654f );

	// functions
	xEnvironment.GetVariable( "+" ) = EvaluationResult( "+", Addition );
	xEnvironment.GetVariable( "-" ) = EvaluationResult( "-", Subtraction );
	xEnvironment.GetVariable( "*" ) = EvaluationResult( "*", Multiplication );

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
