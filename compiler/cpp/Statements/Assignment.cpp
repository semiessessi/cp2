// Copyright (c) 2019 Cranium Software

#include "Assignment.h"

#include "../Context.h"
#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

Assignment::Assignment(
    const std::string& xVariableName,
    const ASTNode* const pxExpression )
: mxVariableName( xVariableName )
, mpxStringExpression( pxExpression )
{
}

void Assignment::Execute( Context& xContext )
{
    const std::string xResult
        = EvaluateStringExpression( mpxStringExpression, xContext );
    xContext.UpdateVariable( mxVariableName, xResult );
}

PassStatement* Assignment::Clone() const
{
    return new Assignment( mxVariableName, mpxStringExpression );
}

PassStatement* Assignment::Create( const ASTNode* const pxAST )
{
    if( pxAST->GetChildCount() != 4 )
    {
        // SE - TODO: error.
        return nullptr;
    }

    if( pxAST->GetChild( 1 )->GetProductionName() != "=" )
    {
        return nullptr;
    }

    return new Assignment(
        pxAST->GetChild( 0 )->GetTokenValue(),
        pxAST->GetChild( 2 ) );
}

}
}
