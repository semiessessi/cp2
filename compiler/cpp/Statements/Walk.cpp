// Copyright (c) 2019 Cranium Software

#include "Walk.h"

#include "../Context.h"
#include "../WalkStatement.h"
#include "../Variables/ParseVariable.h"
#include "../Variables/Variable.h"
#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

Walk::Walk(
    const ASTNode* const pxExpression,
    const std::string& xIdentifier,
    const ASTNode* const pxScope )
: mpxParseExpression( pxExpression )
, mpxStatements( pxScope )
, mxName( xIdentifier )
, mpxScope( WalkStatement::Create( pxScope ) )
{
}

Walk::~Walk()
{
    delete mpxScope;
}

void Walk::Execute( Context& xContext )
{
    Variable* const pxMaybeParse =
        EvaluateParseExpression( mpxParseExpression, xContext );
    if( pxMaybeParse == nullptr )
    {
        // SE - TODO: error???
        return;
    }

    ParseVariable* const pxParse =
        static_cast< ParseVariable* >( pxMaybeParse );

    // stack to add nodes to...
    std::vector< const ASTNode* > xTreeWalkingStack;
    xTreeWalkingStack.push_back( pxParse->GetNode() );
    while( xTreeWalkingStack.size() > 0 )
    {
        const ASTNode* const pxCurrent = xTreeWalkingStack.back();
        xTreeWalkingStack.pop_back();
        Variable* const pxNew = new ParseVariable(
            "<temporary-parse>", pxCurrent );
        Context xNewContext = xContext.CreateChild();
        xNewContext.UpdateVariable( mxName, pxNew );
        mpxScope->Execute( xNewContext );
        delete pxNew;

        if( xNewContext.GetWalkState().bStopped )
        {
            break;
        }

        const int iChildCount = pxCurrent->GetChildCount();
        for( int i = 0; i <iChildCount; ++i )
        {
            xTreeWalkingStack.push_back(
                pxCurrent->GetChild( iChildCount - i - 1 ) );
        }
    }

    delete pxParse;
}

PassStatement* Walk::Clone() const
{
    return new Walk( mpxParseExpression, mxName, mpxStatements );
}

PassStatement* Walk::Create( const ASTNode* const pxAST )
{
    return new Walk(
        pxAST->GetChild( 1 ),
        pxAST->GetChild( 3 )->GetTokenValue(),
        pxAST->GetChild( 4 ) );
}

}
}
