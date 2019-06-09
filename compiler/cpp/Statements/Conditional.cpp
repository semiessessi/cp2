// Copyright (c) 2019 Cranium Software

#include "Conditional.h"

#include "../Context.h"
#include "../Variables/Variable.h"
#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

Conditional::Conditional(
    const ASTNode* const pxExpression,
    const PassStatement* const pxIf,
    const PassStatement* const pxElse )
: mpxBooleanExpression( pxExpression )
, mpxIf( pxIf->Clone() )
, mpxElse( pxElse ? pxElse->Clone() : nullptr )
{
}

Conditional::~Conditional()
{
    delete mpxIf;
    delete mpxElse;
}

void Conditional::Execute( Context& xContext )
{
    if( EvaluateBooleanExpression( mpxBooleanExpression, xContext ) )
    {
        mpxIf->Execute( xContext );
    }
    else if( mpxElse != nullptr )
    {
        mpxElse->Execute( xContext );
    }
}

PassStatement* Conditional::Clone() const
{
    return new Conditional( mpxBooleanExpression, mpxIf, mpxElse );
}

PassStatement* Conditional::Create( const ASTNode* const pxAST )
{
    return new Conditional(
        pxAST->GetChild( 1 ),
        PassStatement::Create( pxAST->GetChild( 2 ) ),
        ( pxAST->GetChildCount() < 5 )
            ? nullptr
            : PassStatement::Create(
                pxAST->GetChild( 4 ) ) );
}

}
}
