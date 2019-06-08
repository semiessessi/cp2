// Copyright (c) 2019 Cranium Software

#include "PassStatement.h"

#include "Context.h"
#include "Statements/Assignment.h"
#include "Statements/ForEach.h"
#include "Statements/Output.h"
#include "Statements/PassScope.h"
#include "Statements/Write.h"
#include "Variables/ArrayVariable.h"
#include "Variables/ProductionVariable.h"
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
    PassStatement* (*)( const ASTNode* const ) >
        kaxActionMap =
{
    { "{", PassScope::Create },
    { "output", Output::Create },
    { "write", Write::Create },
    { "for", ForEach::Create },
};

#undef BP

PassStatement* PassStatement::Create( const ASTNode* const pxAST )
{
    std::unordered_map<
        std::string,
        PassStatement* (*)( const ASTNode* const ) >::const_iterator
            xLookupIterator = kaxActionMap.find(
                pxAST->GetChild( 0 )->GetProductionName() );
    if( xLookupIterator != kaxActionMap.end() )
    {
        return xLookupIterator->second( pxAST );
    }

    PassStatement* pxStatement = nullptr;
    pxStatement = Assignment::Create( pxAST );
    return pxStatement;
}

Variable* PassStatement::EvaluateArrayExpression(
    const ASTNode* const pxAST, const Context& xContext )
{
    if( pxAST->GetChildCount() == 3 )
    {
        // special cases
        if( pxAST->GetChild( 1 )->GetProductionName() == "." )
        {
            if( pxAST->GetChild( 0 )->GetProductionName() == "language" )
            {
                if( pxAST->GetChild( 2 )->GetProductionName() == "productions" )
                {
                    std::vector< Variable* > axProductions;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetProductionCount(); ++i )
                    {
                        axProductions.push_back(
                            new ProductionVariable(
                                "<temporary-production>", xGrammar, i ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-productions>", axProductions );
                }
            }
        }
    }

    // error?

    return nullptr;
}

std::string PassStatement::EvaluateStringExpression(
    const ASTNode* const pxAST, const Context& xContext )
{
    if( pxAST->GetChildCount() == 1 )
    {
        const std::string& xProductionName
            = pxAST->GetChild( 0 )->GetProductionName();
        if( xProductionName == "<string>" )
        {
            const std::string& xValue
                = pxAST->GetChild( 0 )->GetTokenValue();
            return xValue.substr( 1, xValue.length() - 2 );
        }
        else if( xProductionName == "<identifier>" )
        {
            const Variable* const pxVariable
                = xContext.GetVariable(
                    pxAST->GetChild( 0 )->GetTokenValue() );
            if( pxVariable )
            {
                return pxVariable->GetValue();
            }

            // error, unidentified variable.
        }
    }
    else if( pxAST->GetChildCount() == 3 )
    {
        if( pxAST->GetChild( 1 )->GetProductionName() == "+" )
        {
            // concatenating...
            return EvaluateStringExpression( pxAST->GetChild( 0 ), xContext )
                + EvaluateStringExpression( pxAST->GetChild( 2 ), xContext );
        }
        else if( pxAST->GetChild( 1 )->GetProductionName() == "." )
        {
            const Variable* pxVariable
                = xContext.GetVariable(
                    pxAST->GetChild( 0 )->GetTokenValue() );
            if( pxVariable == nullptr )
            {
                pxVariable = xContext.GetVariable(
                    pxAST->GetChild( 0 )->GetTokenName() );
            }
            if( pxVariable )
            {
                if( pxAST->GetChild( 2 )->GetProductionName() == "name" )
                {
                    return pxVariable->GetNameValue();
                }
            }
        }
    }

    return "<error>";
}

}
}
