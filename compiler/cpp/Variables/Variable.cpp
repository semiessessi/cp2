// Copyright (c) 2019 Cranium Software

#include "Variable.h"

#include "../../../common/cpp/Report.h"

namespace CP2
{
namespace Compiler
{

Variable::Variable( const std::string& xName )
: mxName( xName )
{

}

std::string Variable::GetValue() const
{
    CP2::Error( 6000, "???", 0, 0, "Reading from variable which can not be treated as a string." );
    return "<error>";
}

}
}
