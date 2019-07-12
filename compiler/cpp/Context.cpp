// Copyright (c) 2019 Cranium Software

#include "Context.h"

#include "Pass.h"
#include "Variables/LanguageVariable.h"
#include "Variables/StringVariable.h"

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

    for( auto xPair : mxVariables )
    {
        delete xPair.second;
    }
}

Context Context::CreateChild()
{
    Context xNewContext( this );
    xNewContext.mpxParentContext = this;
    return xNewContext;
}

Context Context::CreateForPass(
    const Pass& xPass,
    const Parser::Grammar& xGrammar,
    const Passes& xPasses )
{
    Context xNewContext;
    xNewContext.mpxGrammar = &xGrammar;
    xNewContext.mxVariables[ "pass-name" ]
        = new StringVariable( "pass-name", xPass.GetName() );
    xNewContext.mxVariables[ "language" ]
        = new LanguageVariable( xGrammar );
    xNewContext.mpxPasses = &xPasses;

    return xNewContext;
}

void Context::SetCurrentFile( const std::string& xPath )
{
    void* pFile = GetFileFromPath( xPath );
    if( pFile == nullptr )
    {
        pFile = fopen( xPath.c_str(), "wb" );
    }

    if( pFile != nullptr )
    {
        mxFiles[ xPath ] = pFile;
        mxCurrentPath = xPath;
        mpCurrentFile = pFile;
    }
}

Variable* Context::GetVariable( const std::string& xName )
{
    auto pFound = mxVariables.find( xName );
    return ( pFound == mxVariables.end() )
        ? ( mpxParentContext
            ? mpxParentContext->GetVariable( xName )
            : nullptr ) : pFound->second;
}

const Variable* Context::GetVariable( const std::string& xName ) const
{
    auto pFound = mxVariables.find( xName );
    return ( pFound == mxVariables.end() )
        ? ( mpxParentContext
            ? mpxParentContext->GetVariable( xName )
            : nullptr ) : pFound->second;
}

void Context::UpdateVariable(
    const std::string& xName,
    const std::string& xValue,
    const bool bLocal )
{
    UpdateVariable( xName, new StringVariable( xName, xValue ), bLocal );
}

void Context::UpdateVariable(
    const std::string& xName,
    const Variable* const pxVariable,
    const bool bLocal )
{
    if( !bLocal )
    {
        auto pFound = mxVariables.find( xName );
        if( ( pFound == mxVariables.end() ) && ( mpxParentContext != nullptr ) )
        {
            mpxParentContext->UpdateVariable( xName, pxVariable, false );
            return;
        }
    }

    Variable* const pxNew = pxVariable->Clone();
    delete mxVariables[ xName ];
    mxVariables[ xName ] = pxNew;
}

Context::Context()
: mpCurrentFile( nullptr )
, mpxParentContext( nullptr )
, mpxGrammar( nullptr )
{
}

Context::Context( const Context& xOther )
: mxFiles( xOther.mxFiles )
//, mxVariables( xOther.mxVariables )
, mxCurrentPath( xOther.mxCurrentPath )
, mpCurrentFile( xOther.mpCurrentFile )
, mpxParentContext( xOther.mpxParentContext )
, mpxGrammar( xOther.mpxGrammar )
{
    for( auto xPair : xOther.mxVariables )
    {
        mxVariables[ xPair.first ] = xPair.second->Clone();
    }
}

Context::Context( const Context* const pxOther )
: mxFiles()
, mxVariables()
, mxCurrentPath( pxOther->mxCurrentPath )
, mpCurrentFile( pxOther->mpCurrentFile )
, mpxParentContext( nullptr )
, mpxGrammar( pxOther->mpxGrammar )
{
}

Context& Context::operator=( const Context& xOther )
{
    mxFiles = xOther.mxFiles;
    mxCurrentPath = xOther.mxCurrentPath;
    mpCurrentFile = xOther.mpCurrentFile;
    mpxParentContext = xOther.mpxParentContext;
    mpxGrammar = xOther.mpxGrammar;

    for( auto xPair : xOther.mxVariables )
    {
        mxVariables[ xPair.first ] = xPair.second->Clone();
    }

    return *this;
}

void* Context::GetFileFromPath( const std::string& xPath )
{
    auto pFound = mxFiles.find( xPath );
    if( pFound != mxFiles.end() )
    {
        return pFound->second;
    }

    if( mpxParentContext )
    {
        return mpxParentContext->GetFileFromPath( xPath );
    }

    return nullptr;
}

}
}
