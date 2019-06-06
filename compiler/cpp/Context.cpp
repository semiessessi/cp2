// Copyright (c) 2019 Cranium Software

#include "Context.h"

#include "Pass.h"
#include "StringVariable.h"

#include <cstdio>

namespace CP2
{
namespace Compiler
{

Context::~Context()
{
    for( auto xPair : mxFiles )
    {
        fclose( static_cast< FILE* >( xPair.second ) );
    }
}

Context Context::CreateChild()
{
    Context xNewContext( *this );
    xNewContext.mpxParentContext = this;
    return xNewContext;
}

Context Context::CreateForPass( const Pass& xPass )
{
    Context xNewContext;
    //xNewContext.mxVariables.insert( xPass.GetName() );
    return xNewContext;
}

Context::Context()
: mpCurrentFile( nullptr )
, mpxParentContext( nullptr )
{
}

Context::Context( const Context& xOther )
: mxFiles()
, mxVariables()
, mxCurrentPath( xOther.mxCurrentPath )
, mpCurrentFile( xOther.mpCurrentFile )
, mpxParentContext( xOther.mpxParentContext )
{
}

}
}
