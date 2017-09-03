// Copyright (c) 2017 Cranium Software

#ifndef CP2PARSER_H
#define CP2PARSER_H

#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/Token.h"

#include <string>
#include <vector>

namespace CP2
{
namespace Parser
{

ASTNode* CP2FirstPassParse( const std::vector< Token >& axTokens );
ASTNode* CP2Parse( const std::vector< Token >& axTokens );

}
}

#endif
