// Copyright (c) 2019 Cranium Software

#include "PassStatement.h"

#include "Context.h"
#include "Statements/Assignment.h"
#include "Statements/Conditional.h"
#include "Statements/ForEach.h"
#include "Statements/Output.h"
#include "Statements/PassScope.h"
#include "Statements/Write.h"
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
    PassStatement* (*)( const ASTNode* const ) >
        kaxActionMap =
{
    { "{", PassScope::Create },
    { "output", Output::Create },
    { "write", Write::Create },
    { "for", ForEach::Create },
    { "if", Conditional::Create },
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
    if( pxAST->GetChildCount() == 1 )
    {
        if( pxAST->GetChild( 0 )->GetProductionName() == "<identifier>" )
        {
            return xContext.GetVariable( pxAST->GetChild( 0 )->GetTokenValue() )->Clone();
        }
    }

    if( pxAST->GetChildCount() == 3 )
    {
        // special cases
        if( pxAST->GetChild( 1 )->GetProductionName() == "." )
        {
            if( pxAST->GetChild( 0 )->GetProductionName() == "language" )
            {
                if( pxAST->GetChild( 2 )->GetProductionName() == "passes" )
                {
                    std::vector< Variable* > axValues;
                    const Passes& xPasses
                        = *( xContext.GetPasses() );
                    for( int i = 0; i < xPasses.GetPassCount(); ++i )
                    {
                        axValues.push_back(
                            new PassVariable(
                                "<temporary-pass>", xPasses, i ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-passes>", axValues, true );
                }

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
                        "<temporary-language-productions>", axProductions, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "lexemes" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetLexemeCount(); ++i )
                    {
                        axValues.push_back(
                            new LexemeVariable(
                                "<temporary-lexeme>", xGrammar, i ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-lexemes>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "quotes" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( size_t i = 0; i < xGrammar.GetQuotes().size(); ++i )
                    {
                        axValues.push_back(
                            new QuoteVariable(
                                "<temporary-quote>", xGrammar, static_cast< int >( i ) ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-quotes>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "keywords" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetKeywordCount(); ++i )
                    {
                        axValues.push_back(
                            new StringVariable(
                                "<temporary-identifier>",
                                xGrammar.GetKeywords()[ i ] ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-keywords>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "identifiers" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetIdentifierCount(); ++i )
                    {
                        axValues.push_back(
                            new StringVariable(
                                "<temporary-identifier>",
                                xGrammar.GetIdentifiers()[ i ] ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-identifiers>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "operators" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetOperatorCount(); ++i )
                    {
                        axValues.push_back(
                            new StringVariable(
                                "<temporary-identifier>",
                                xGrammar.GetOperators()[ i ] ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-operators>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "terminators" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetTerminatorCount(); ++i )
                    {
                        axValues.push_back(
                            new StringVariable(
                                "<temporary-identifier>",
                                xGrammar.GetTerminators()[ i ] ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-terminators>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "separators" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetSeparatorCount(); ++i )
                    {
                        axValues.push_back(
                            new StringVariable(
                                "<temporary-identifier>",
                                xGrammar.GetSeparators()[ i ] ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-separators>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "strings" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetStringCount(); ++i )
                    {
                        axValues.push_back(
                            new StringVariable(
                                "<temporary-identifier>",
                                xGrammar.GetStrings()[ i ] ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-strings>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "line-comments" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetCommentCount(); ++i )
                    {
                        if( xGrammar.GetComments()[ i ].GetEnd() != nullptr )
                        {
                            continue;
                        }

                        axValues.push_back(
                            new StringVariable(
                                "<temporary-line-comment>",
                                xGrammar.GetComments()[ i ].GetStart() ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-strings>", axValues, true );
                }

                if( pxAST->GetChild( 2 )->GetProductionName() == "block-comments" )
                {
                    std::vector< Variable* > axValues;
                    const Parser::Grammar& xGrammar
                        = *( xContext.GetGrammar() );
                    for( int i = 0; i < xGrammar.GetCommentCount(); ++i )
                    {
                        if( xGrammar.GetComments()[ i ].GetEnd() == nullptr )
                        {
                            continue;
                        }

                        const StringVariable xFirst( "<temporary-block-comment-start>",
                            xGrammar.GetComments()[ i ].GetStart() );
                        const StringVariable xLast( "<temporary-block-comment-end>",
                            xGrammar.GetComments()[ i ].GetEnd() );
                        axValues.push_back(
                            new PairVariable(
                                "<temporary-block-comment>",
                                &xFirst,
                                &xLast ) );
                    }
                    return new ArrayVariable(
                        "<temporary-language-block-comments>", axValues, true );
                }
                // error?
            }

            if( pxAST->GetChild( 0 )->GetProductionName() == "<identifier>" )
            {
                if( pxAST->GetChild( 2 )->GetProductionName() == "names" )
                {
                    const Variable* const pxVariable
                        = xContext.GetVariable(
                            pxAST->GetChild( 0 )->GetTokenValue() );

                    if( pxVariable->HasNames() == true )
                    {
                        return pxVariable->Clone();
                    }

                    // error?
                }

                // error?
            }
        }

    }

    // error?

    return nullptr;
}

bool PassStatement::EvaluateBooleanExpression(
    const ASTNode* const pxAST, const Context& xContext )
{
    if( pxAST->GetChildCount() == 1 )
    {
        return pxAST->GetChild( 0 )->GetProductionName() == "true";
    }
    else if( pxAST->GetChildCount() == 3 )
    {
        if( pxAST->GetChild( 1 )->GetProductionName() == "." )
        {
            const Variable* pxVariable
                = xContext.GetVariable(
                    pxAST->GetChild( 0 )->GetTokenValue() );
            if( pxVariable == nullptr )
            {
                pxVariable
                    = xContext.GetVariable(
                        pxAST->GetChild( 0 )->GetProductionName() );
            }

            if( pxVariable != nullptr )
            {
                if( pxAST->GetChild( 2 )->GetProductionName()
                    == "is-optional" )
                {
                    return pxVariable->IsOptionalName();
                }
                else if( pxAST->GetChild( 2 )->GetProductionName()
                    == "is-non-empty" )
                {
                    return pxVariable->IsNonEmptyName();
                }
                else if( pxAST->GetChild( 2 )->GetProductionName()
                    == "is-list" )
                {
                    return pxVariable->IsMultipleName();
                }
            }
        }

        if( pxAST->GetChild( 1 )->GetProductionName() == "==" )
        {
            if( pxAST->GetChild( 0 )->GetProductionName() == "<string-expression>" )
            {
                return EvaluateStringExpression(
                        pxAST->GetChild( 0 ), xContext )
                    == EvaluateStringExpression(
                        pxAST->GetChild( 2 ), xContext );
            }
            else if( pxAST->GetChild( 0 )->GetProductionName() == "<boolean-expression>" )
            {
                return EvaluateBooleanExpression(
                        pxAST->GetChild( 0 ), xContext )
                    == EvaluateBooleanExpression(
                        pxAST->GetChild( 2 ), xContext );
            }
        }
    }

    // SE - TODO: error?
    return false;
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
        else if( xProductionName == "<boolean-expression>" )
        {
            return EvaluateBooleanExpression(
                pxAST->GetChild( 0 ), xContext )
                    ? "true" : "false";
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

                if( pxAST->GetChild( 2 )->GetProductionName() == "input-name" )
                {
                    return pxVariable->GetInputName();
                }
            }
        }
    }

    return "<error>";
}

}
}
