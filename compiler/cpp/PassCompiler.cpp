// Copyright (c) 2019 Cranium Software

#include "PassCompiler.h"

#include "Passes.h"

#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/Report.h"

namespace CP2
{
namespace Compiler
{

Passes CompilePasses( const ASTNode* const pxAST )
{
    Passes xPasses;
    if( pxAST == nullptr )
    {
        Warning( 5501, "<unknown>", 0, 0,
            "Unable to compile passes from empty syntax tree, producing empty result" );
        return xPasses;
    }

    // the top should be <grammar>
    // i feel the parser stops this being hit...
    if( pxAST->GetProductionName() != "<grammar>" )
    {
        Error( 5000, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
            "Expected <grammar> but found %s instead", pxAST->GetProductionName().c_str() );
        return xPasses;
    }

    const int iChildCount = pxAST->GetChildCount();
    if( iChildCount == 0 )
    {
        Warning( 5500, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
            "No compiler passes found" );
        return xPasses;
    }

    for( int i = 0; i < iChildCount; ++i )
    {
        ASTNode* const pxStatementAST = pxAST->GetChild( i );
        const int iStatementChildCount = pxStatementAST->GetChildCount();

        // i feel the parser stops this being hit...
        if( iStatementChildCount <= 0 )
        {
            Error( 5001, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
                "Unexpectedly short statement found: %s", pxStatementAST->GetTokenValue().c_str() );
            continue;
        }

        // the first child should be the name, unless it is a special case
        // like language, comment, quote or lexeme
        ASTNode* const pxPotentialPass = pxStatementAST->GetChild( 0 );
        const int iPassChildCount = pxPotentialPass->GetChildCount();
        // SE - TODO: these things need to be in data in some map...
        if( pxPotentialPass->GetProductionName() != "<pass-definition>" )
        {
            continue;
        }

        const std::string& xPassName = pxPotentialPass->GetChild( 1 )->GetTokenValue();
        // add pass name, stripping quotes
        Pass& xPass = xPasses.AddPass( xPassName.substr( 1, xPassName.length() - 2 ) );
    }

    return xPasses;
}

}
}
