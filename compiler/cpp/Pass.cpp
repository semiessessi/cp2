// Copyright (c) 2019 Cranium Software

#include "Pass.h"

#include "Context.h"
#include "PassStatement.h"
#include "Statements/PassScope.h"

namespace CP2
{
namespace Compiler
{

Pass::Pass()
: mbIsSwitch( false )
, mpxStatements( nullptr )
{

}

Pass::Pass( const Pass& xPass )
: mbIsSwitch( xPass.mbIsSwitch )
, mpxStatements( xPass.mpxStatements
    ? new PassScope( xPass.mpxStatements )
    : nullptr )
{

}

Pass::~Pass()
{
    delete mpxStatements;
}

Pass& Pass::operator =( const Pass& xPass )
{
    mbIsSwitch = xPass.mbIsSwitch;
    return *this;
}

void Pass::Compile( const ASTNode* const pxAST )
{
    delete mpxStatements;
    mpxStatements = static_cast< PassScope* >(
        PassStatement::Create( pxAST ) );
}

void Pass::GetRequiredPaths( std::vector< OutputFile >& xFiles )
{
    if( mpxStatements )
    {
        mpxStatements->GetRequiredPaths( xFiles );
    }
}

void Pass::Execute()
{
    if( mpxStatements )
    {
        Context xPassContext = Context::CreateForPass( *this );
        mpxStatements->Execute( xPassContext );
    }
}

}
}
