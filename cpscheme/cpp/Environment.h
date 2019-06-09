// Copyright (c) 2017 Cranium Software

#ifndef CPS_ENVIRONMENT_H
#define CPS_ENVIRONMENT_H

#include <string>
#include <unordered_map>

namespace CP2
{
namespace Scheme
{

class EvaluationResult;

class Environment
{

public:

	~Environment();
	Environment( const Environment& xOther );

	static Environment& GlobalEnvironment() { return sxGlobalEnvironment; }
	static Environment NewDefaultEnvironment();

	bool VariableIsDefined( const char* const szName ) const;
	EvaluationResult& GetVariable( const char* const szName );

private:

	Environment() {}

	static Environment sxGlobalEnvironment;

	std::unordered_map< std::string, EvaluationResult* > mxVariables;

};

}
}

#endif
