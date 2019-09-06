// Copyright (c) 2019 Cranium Software

#include "WalkStatement.h"

#include "Context.h"
#include "Passes.h"
#include "PassStatement.h"
#include "Statements/Walk/WalkScope.h"
#include "Statements/Walk/WrappedPassStatement.h"
#include "Variables/ArrayVariable.h"
#include "Variables/LexemeVariable.h"
#include "Variables/PairVariable.h"
#include "Variables/PassVariable.h"
#include "Variables/ProductionVariable.h"
#include "Variables/QuoteVariable.h"
#include "Variables/StringVariable.h"
#include "Variables/Variable.h"

#include "../../common/cpp/ASTNode.h"
#include "../../parser/cpp/Grammar.h"

namespace CP2
{
namespace Compiler
{

// boilerplate... in case we want to do more than use a static function
#define BP []( const ASTNode* const pxAST ) -> PassStatement*

const std::unordered_map<
    std::string,
    WalkStatement* (*)( const ASTNode* const ) >
        kaxWalkActionMap =
{
    { "{", WalkScope::Create },
    /*
    { "break", WalkBreak::Create },
    { "continue", WalkContinue::Create },
    */

};

#undef BP

WalkStatement* WalkStatement::Create( const ASTNode* const pxAST )
{
    std::unordered_map<
        std::string,
        WalkStatement* (*)( const ASTNode* const ) >::const_iterator
            xLookupIterator = kaxWalkActionMap.find(
                pxAST->GetChild( 0 )->GetProductionName() );
    if( xLookupIterator != kaxWalkActionMap.end() )
    {
        return xLookupIterator->second( pxAST );
    }

    WalkStatement* pxStatement = nullptr;

    if( pxAST->GetChild( 0 )->GetProductionName()
        == "<pass-statement>" )
    {
        pxStatement = WrappedPassStatement::Create( pxAST->GetChild( 0 ) );
    }

    return pxStatement;
}

}
}
