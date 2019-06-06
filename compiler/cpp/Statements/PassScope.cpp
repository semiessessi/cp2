// Copyright (c) 2019 Cranium Software

#include "PassScope.h"

#include "../Context.h"
#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

PassScope::PassScope( const PassScope* const pxOther )
: maxStatements()
{
    if( pxOther == nullptr )
    {
        return;
    }

    for( PassStatement* const pxStatement : pxOther->maxStatements )
    {
        maxStatements.push_back( pxStatement->Clone() );
    }
}

PassScope::~PassScope()
{
    for( PassStatement* const pxStatement : maxStatements )
    {
        delete pxStatement;
    }
}

void PassScope::Execute( Context& xContext )
{
    // create a child context for this.
    Context xScopeContext = xContext.CreateChild();
    // SE - TODO: parallel goodies?
    for( PassStatement* const pxStatement : maxStatements )
    {
        pxStatement->Execute( xContext );
    }
}

PassStatement* PassScope::Clone() const
{
    return new PassScope( this );
}

PassStatement* PassScope::Create( const ASTNode* const pxAST )
{
    PassScope* const pxNewScope = new PassScope( nullptr );
    const int iStatementCount = pxAST->GetChildCount() - 2;
    for( int i = 1; i <= iStatementCount; ++i )
    {
        PassStatement* const pxNewStatement
            = PassStatement::Create( pxAST->GetChild( i ) );
        if( pxNewStatement ) // SE - TODO: error?
        {
            pxNewScope->maxStatements.push_back( pxNewStatement );
        }
    }

    return pxNewScope;
}

}
}
