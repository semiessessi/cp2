// Copyright (c) 2017 Cranium Software

#ifndef CBNF_PARSER_H
#define CBNF_PARSER_H

#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/Token.h"

#include <string>
#include <vector>

namespace CP2
{
namespace Parser
{

class Grammar;

const Grammar& GetCBNFGrammar();
ASTNode* CBNFParse( const std::vector< Token >& axTokens );

}
}

#endif
