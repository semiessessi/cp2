// Copyright (c) 2017 Cranium Software

#ifndef PARSER_H
#define PARSER_H

#include "../../common/cpp/Token.h"

#include <string>
#include <vector>

namespace CP2
{

class ASTNode;

namespace Parser
{

class Grammar;

ASTNode* Parse( const std::vector< Token >& axTokens, const Grammar& xGrammar );

}
}

#endif
