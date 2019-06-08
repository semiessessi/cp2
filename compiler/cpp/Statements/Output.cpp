// Copyright (c) 2019 Cranium Software

#include "Output.h"

#include "../Context.h"
#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

Output::Output( const std::string& xPath )
: mxPath( xPath )
, mpxStringExpression( nullptr )
{
}

Output::Output( const ASTNode* const pxExpression )
: mxPath()
, mpxStringExpression( pxExpression )
{
}

Output::~Output()
{
}

void Output::Execute( Context& xContext )
{
    if( mxPath.empty() && ( mpxStringExpression != nullptr ) )
    {
        mxPath = EvaluateStringExpression( mpxStringExpression, xContext );
    }

    xContext.SetCurrentFile( mxPath );
}

PassStatement* Output::Clone() const
{
    return mpxStringExpression
        ? new Output( mpxStringExpression )
        : new Output( mxPath );
}

void Output::GetRequiredPaths( std::vector< OutputFile >& xFiles ) const
{
    const OutputFile xOutputFile( mxPath, mbBinary );
    xFiles.push_back( xOutputFile );
}

PassStatement* Output::Create( const ASTNode* const pxAST )
{
    // SE - NOTE: this treats the constants specially
    const ASTNode* const pxExpression = pxAST->GetChild( 1 );
    if( ( pxExpression->GetChildCount() == 1 )
        && pxExpression->GetChild( 0 )->GetProductionName() == "<string>" )
    {
        const std::string xPath = pxAST->GetChild( 1 )->GetChild( 0 )->GetTokenValue();
        return new Output( xPath.substr( 1, xPath.size() - 2 ) );
    }
    
    return new Output( pxExpression );
}

}
}
