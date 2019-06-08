// Copyright (c) 2019 Cranium Software

#include "Write.h"

#include "../Context.h"
#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

Write::Write( const ASTNode* const pxExpression )
: mpxStringExpression( pxExpression )
{
}

void Write::Execute( Context& xContext )
{
    FILE* pFile = static_cast< FILE* >(
        xContext.GetCurrentFile() );
    if( pFile != nullptr )
    {
        // SE - TODO: better...
        fputs( EvaluateStringExpression(
            mpxStringExpression, xContext ).c_str(), pFile );
    }
}

PassStatement* Write::Clone() const
{
    return new Write( mpxStringExpression );
}

PassStatement* Write::Create( const ASTNode* const pxAST )
{
    return new Write( pxAST->GetChild( 1 ) );
}

}
}
