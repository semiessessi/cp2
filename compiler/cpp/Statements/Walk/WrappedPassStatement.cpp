// Copyright (c) 2019 Cranium Software

#include "WrappedPassStatement.h"

#include "../../PassStatement.h"

namespace CP2
{
namespace Compiler
{

WrappedPassStatement::WrappedPassStatement(
    PassStatement* const pxPassStatement )
: mpxPassStatement( pxPassStatement )
{

}

WrappedPassStatement::~WrappedPassStatement()
{
    delete mpxPassStatement;
}

void WrappedPassStatement::Execute( Context& xContext )
{
    // SE - NOTE: paranoia?!?
    if( mpxPassStatement )
    {
        mpxPassStatement->Execute( xContext );
    }
}

WalkStatement* WrappedPassStatement::Clone() const
{
    return new WrappedPassStatement( mpxPassStatement->Clone() );
}

WalkStatement* WrappedPassStatement::Create(
    const ASTNode* const pxAST )
{
    PassStatement* pxPassStatement =
        PassStatement::Create( pxAST );
    return new WrappedPassStatement( pxPassStatement );
}

}
}