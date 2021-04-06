// Copyright (c) 2019 Cranium Software

#include "Assignment.h"

#include "../Context.h"
#include "../Variables/BooleanVariable.h"
#include "../Variables/IntegerVariable.h"
#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

Assignment::Assignment(
    const std::string& xVariableName,
    const ASTNode* const pxExpression )
: mxVariableName( xVariableName )
, mpxExpression( pxExpression )
{
}

void Assignment::Execute( Context& xContext )
{
    if( mpxExpression->GetProductionName() == "<string-expression>" )
    {
        const std::string xResult
            = EvaluateStringExpression( mpxExpression, xContext );
        xContext.UpdateVariable( mxVariableName, xResult );
    }
    else if( mpxExpression->GetProductionName() == "<boolean-expression>" )
    {
        const BooleanVariable xResult( mxVariableName,
            EvaluateBooleanExpression( mpxExpression, xContext ) );
        xContext.UpdateVariable(
            mxVariableName,
            &xResult );
    }
    else if (mpxExpression->GetProductionName() == "<integer-expression>")
    {
        const IntegerVariable xResult(mxVariableName,
            EvaluateIntegerExpression(mpxExpression, xContext));
        xContext.UpdateVariable(
            mxVariableName,
            &xResult);
    }
}

PassStatement* Assignment::Clone() const
{
    return new Assignment( mxVariableName, mpxExpression );
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
