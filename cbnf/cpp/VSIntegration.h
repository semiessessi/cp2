// Copyright (c) 2017 Cranium Software

#ifndef VS_INTEGRATION_H
#define VS_INTEGRATION_H

#include <string>

namespace CP2
{
namespace Parser
{

class Grammar;

}

struct VSIntegrationParameters
{
	bool mb;
};

void CreateIntegrationInPath(
	const char* const szPath,
	const Parser::Grammar& xGrammar,
	const VSIntegrationParameters& xParameters = VSIntegrationParameters() );

// helpers
std::string GetProjectGuid( const size_t i );
//std::string GetProjectTypeGuid( const Parser::Grammar& xGrammar );
}

#endif

