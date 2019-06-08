// Copyright (c) 2019 Cranium Software

#include "ForEach.h"

#include "../Context.h"
#include "../Variables/Variable.h"
#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

ForEach::ForEach(
    const ASTNode* const pxExpression,
    const std::string& xIdentifier,
    const ASTNode* const pxScope )
: mpxArrayExpression( pxExpression )
, mpxStatements( pxScope )
, mxName( xIdentifier )
, mpxScope( PassStatement::Create( pxScope ) )
{
}

void ForEach::Execute( Context& xContext )
{
    Variable* pxVariable =
        EvaluateArrayExpression( mpxArrayExpression, xContext );
    if( pxVariable == nullptr )
    {
        // SE - TODO: error???
        return;
    }

    for( int i = 0; i < pxVariable->GetIndexedCount(); ++i )
    {
        Variable* const pxNew = pxVariable->GetIndexed( i );
        Context xNewContext = xContext.CreateChild();
        xNewContext.UpdateVariable( mxName, pxNew );
        mpxScope->Execute( xNewContext );
    }
}

PassStatement* ForEach::Clone() const
{
    return new ForEach( mpxArrayExpression, mxName, mpxStatements );
}

PassStatement* ForEach::Create( const ASTNode* const pxAST )
{
    return new ForEach(
        pxAST->GetChild( 4 ),
        pxAST->GetChild( 2 )->GetTokenValue(),
        pxAST->GetChild( 5 ) );
}

}
}
