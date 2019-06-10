// Copyright (c) 2019 Cranium Software

#include "ArrayVariable.h"

namespace CP2
{
namespace Compiler
{

ArrayVariable::ArrayVariable(
    const std::string& xName,
    const std::vector< Variable* >& xArray,
    const bool bDelete )
: Variable( xName )
, mxContent()
{
    for( const Variable* const pxVariable : xArray )
    {
        mxContent.push_back( pxVariable->Clone() );
    }

    if( bDelete )
    {
        DeleteArray( xArray );
    }
}

ArrayVariable::~ArrayVariable()
{
    DeleteArray( mxContent );
}

}
}
