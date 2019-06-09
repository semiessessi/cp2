// Copyright (c) 2019 Cranium Software

#include "IntegerVariable.h"

namespace CP2
{
namespace Compiler
{

IntegerVariable::IntegerVariable(
    const std::string& xName,
    const int iValue )
: Variable( xName )
, miValue( iValue )
{

}

}
}
