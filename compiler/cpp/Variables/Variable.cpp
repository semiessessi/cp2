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
    CP2::Error( 6000, "???", 0, 0, "Reading from variable %s which can not be treated as a string.", mxName.c_str() );
    return "<error-string-value>";
}

Variable* Variable::GetIndexed( const int ) const
{
    CP2::Error( 6001, "???", 0, 0, "Trying to index a variable %s which can not be indexed.", mxName.c_str() );
    return nullptr;
}

int Variable::GetIndexedCount() const
{
    CP2::Error( 6002, "???", 0, 0, "Trying to count a variable %s which can not be indexed.", mxName.c_str() );
    return 0;
}

}
}
