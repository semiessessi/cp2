// Copyright (c) 2019 Cranium Software

#include "BooleanVariable.h"

namespace CP2
{
namespace Compiler
{

BooleanVariable::BooleanVariable(
    const std::string& xName,
    const bool bValue )
: Variable( xName )
, mbValue( bValue )
{

}

}
}
