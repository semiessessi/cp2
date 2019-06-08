// Copyright (c) 2019 Cranium Software

#include "PassStatement.h"

#include "Context.h"
#include "Statements/Assignment.h"
#include "Statements/Output.h"
#include "Statements/PassScope.h"
#include "Variables/Variable.h"

#include "../../common/cpp/ASTNode.h"

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

    return "<error>";
}

}
}
