// Copyright (c) 2019 Cranium Software

#include "PassVariable.h"

#include "NameVariable.h"
#include "ParseVariable.h"
#include "../Passes.h"
#include "../Pass.h"

namespace CP2
{
namespace Compiler
{

PassVariable::PassVariable(
    const std::string& xName,
    const Passes& xPasses,
    const int iIndex )
: Variable( xName )
, mxPasses( xPasses )
, miIndex( iIndex )
{

}

std::string PassVariable::GetNameValue() const
{
    return mxPasses.GetPass( miIndex ).GetName();
}

std::string PassVariable::GetInputName() const
{
    return InputNameFromOutputName( GetNameValue() );
}

Variable* PassVariable::GetParse() const
{
    return new ParseVariable(
        "<temporary-parse-variable>",
        mxPasses.GetPass( miIndex ).GetNode() );
}

}
}
