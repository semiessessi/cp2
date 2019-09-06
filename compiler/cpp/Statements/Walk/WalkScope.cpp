// Copyright (c) 2019 Cranium Software

#include "WalkScope.h"

#include "../../Context.h"
#include "../../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

WalkScope::WalkScope( const WalkScope* const pxOther )
: maxStatements()
{
    if( pxOther == nullptr )
    {
        return;
    }

    for( WalkStatement* const pxStatement : pxOther->maxStatements )
    {
        maxStatements.push_back( pxStatement->Clone() );
    }
}

WalkScope::~WalkScope()
{
    for( WalkStatement* const pxStatement : maxStatements )
    {
        delete pxStatement;
    }
}

void WalkScope::Execute( Context& xContext )
{
    // create a child context for this.
    Context xScopeContext = xContext.CreateChild();
    // SE - TODO: parallel goodies?
    for( WalkStatement* const pxStatement : maxStatements )
    {
        pxStatement->Execute( xContext );

        const ParseWalkState& xWalkState = xContext.GetWalkState();
        if( xWalkState.bSkipped || xWalkState.bStopped )
        {
            return;
        }
    }

    // propogate breaks and continues, at the top level
    // getting here means the walk is already over.
    const ParseWalkState& xWalkState = xContext.GetWalkState();
    if( xWalkState.bStopped )
    {
        xContext.WalkBreak();
    }

    if( xWalkState.bSkipped )
    {
        xContext.WalkContinue();
    }
}

WalkStatement* WalkScope::Clone() const
{
    return new WalkScope( this );
}

WalkStatement* WalkScope::Create( const ASTNode* const pxAST )
{
    WalkScope* const pxNewScope = new WalkScope( nullptr );
    const int iStatementCount = pxAST->GetChildCount() - 2;
    for( int i = 1; i <= iStatementCount; ++i )
    {
        WalkStatement* const pxNewStatement
            = WalkStatement::Create( pxAST->GetChild( i ) );
        if( pxNewStatement ) // SE - TODO: error?
        {
            pxNewScope->maxStatements.push_back( pxNewStatement );
        }
    }

    return pxNewScope;
}

}
}
