// Copyright (c) 2019 Cranium Software

#include "StringVariable.h"

namespace CP2
{
namespace Compiler
{
StringVariable::StringVariable(
    const std::string& xName,
    const std::string& xValue )
: Variable( xName )
, mxValue( xValue )
{

}
}
}