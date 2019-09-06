// Copyright (c) 2019 Cranium Software

#include "Pass.h"

#include "Context.h"
#include "PassStatement.h"
#include "Statements/PassScope.h"

namespace CP2
{
namespace Compiler
{

Pass::Pass( const std::string& xName )
: mbIsSwitch( false )
, mpxStatements( nullptr )
, mxName( xName )
, mpxOriginalNode( nullptr )
{

}

Pass::Pass( const Pass& xPass )
: mbIsSwitch( xPass.mbIsSwitch )
, mpxStatements( xPass.mpxStatements
    ? new PassScope( xPass.mpxStatements )
    : nullptr )
, maxRequiredPasses( xPass.maxRequiredPasses )
, mxName( xPass.mxName )
, mpxOriginalNode( xPass.mpxOriginalNode )
{

}

Pass::~Pass()
{
    delete mpxStatements;
}

Pass& Pass::operator =( const Pass& xPass )
{
    mbIsSwitch = xPass.mbIsSwitch;
    mpxStatements = xPass.mpxStatements
        ? new PassScope( xPass.mpxStatements )
        : nullptr;
    maxRequiredPasses = xPass.maxRequiredPasses;
    mxName = xPass.mxName;
    mpxOriginalNode = xPass.mpxOriginalNode;
    return *this;
}

void Pass::AddRequiredPass( const std::string& xPassName )
{
    if( std::find(
        maxRequiredPasses.begin(),
        maxRequiredPasses.end(),
        xPassName ) == maxRequiredPasses.end() )
    {
        maxRequiredPasses.push_back( xPassName );
    }
}

void Pass::Compile( const ASTNode* const pxAST )
{
    mpxOriginalNode = pxAST;
    delete mpxStatements;
    mpxStatements = static_cast< PassScope* >(
        PassStatement::Create( pxAST ) );
}

void Pass::GetRequiredPaths( std::vector< OutputFile >& xFiles )
{
    if( mpxStatements )
    {
        mpxStatements->GetRequiredPaths( xFiles );

        CreateRequiredPaths( xFiles );
    }
}
void Pass::GetRequiredPasses( std::vector< std::string >& xPassNames )
{
    for( const std::string xPassName : maxRequiredPasses )
    {
        if( std::find(
            maxRequiredPasses.begin(),
            maxRequiredPasses.end(),
            xPassName ) == maxRequiredPasses.end() )
        {
            xPassNames.push_back( xPassName );
        }
    }
}

void Pass::Execute(
    const Parser::Grammar& xGrammar,
    const Passes& xPasses )
{
    if( mpxStatements )
    {
        Context xPassContext =
            Context::CreateForPass( *this, xGrammar, xPasses );
        mpxStatements->Execute( xPassContext );
    }
}

void Pass::CreateRequiredPaths( const std::vector< OutputFile >& xFiles )
{

}

}
}
