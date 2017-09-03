// Copyright (c) 2017 Cranium Software

#ifndef CBNF_LEXER_H
#define CBNF_LEXER_H

#include "../../common/cpp/Token.h"

#include <vector>

namespace CP2
{
namespace Lexer
{

class Rule;

const std::vector< Rule >& CBNFLexerRules();
std::vector< Token > CBNFLex( const char* const szFilename );

}
}

#endif

