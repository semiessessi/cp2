// Copyright (c) 2019 Cranium Software

#include "ParseVariable.h"

namespace CP2
{

namespace Compiler
{

ParseVariable::ParseVariable(
    const std::string& xName,
    const ASTNode* const pxNode )
: Variable( xName )
{
}

ParseVariable::~ParseVariable()
{

}

}
}
