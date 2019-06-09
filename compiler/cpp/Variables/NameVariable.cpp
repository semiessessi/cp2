// Copyright (c) 2019 Cranium Software

#include "NameVariable.h"

namespace CP2
{
namespace Compiler
{

NameVariable::NameVariable(
    const std::string& xVariableName,
    const Parser::Name& xParseName )
: Variable( xVariableName )
, mxParseName( xParseName )
{

}

}
}
