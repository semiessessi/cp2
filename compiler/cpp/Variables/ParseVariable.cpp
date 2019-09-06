// Copyright (c) 2019 Cranium Software

#include "ParseVariable.h"

#include "../../../common/cpp/ASTNode.h"
#include "../../../common/cpp/Escaping.h"

namespace CP2
{

namespace Compiler
{

ParseVariable::ParseVariable(
    const std::string& xName,
    const ASTNode* const pxNode )
: Variable( xName )
, mpxNode( pxNode )
{
}

ParseVariable::~ParseVariable()
{

}

std::string ParseVariable::GetValue() const
{
    return SlashEscape( mpxNode->IsNonTerminal()
        ? mpxNode->GetTokenValue()
        : ( 
            mpxNode->IsValued()
                ? mpxNode->GetTokenValue()
                : mpxNode->GetProductionName() ) );
}

std::string ParseVariable::GetNameValue() const
{
    return mpxNode->GetTokenName();
}

bool ParseVariable::IsMultipleName() const
{
    return mpxNode->IsNonTerminal();
}

}
}
