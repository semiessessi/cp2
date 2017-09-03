// Copyright (c) 2017 Cranium Software

#ifndef CP2_LEXER_H
#define CP2_LEXER_H

#include "../../common/cpp/Token.h"

#include <vector>

namespace CP2
{
namespace Lexer
{

class Rule;

const std::vector< Rule >& CP2LexerRules();
std::vector< Token > CP2Lex( const char* const szFilename );

}
}

#endif

