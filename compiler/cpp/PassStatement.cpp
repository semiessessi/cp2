// Copyright (c) 2019 Cranium Software

#include "PassStatement.h"

#include "Statements/Output.h"
#include "Statements/PassScope.h"

#include "../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

// boilerplate... in case we want to do more than use a static function
#define BP []( const ASTNode* const pxAST ) -> PassStatement*

const std::unordered_map< std::string, PassStatement* (*)( const ASTNode* const ) > kaxActionMap =
{
    { "{", PassScope::Create },
    { "output", Output::Create },
};

#undef BP

PassStatement* PassStatement::Create( const ASTNode* const pxAST )
{
    std::unordered_map< std::string, PassStatement* ( *)( const ASTNode* const ) >::const_iterator
        xLookupIterator = kaxActionMap.find( pxAST->GetChild( 0 )->GetProductionName() );
    if( xLookupIterator != kaxActionMap.end() )
    {
        return xLookupIterator->second( pxAST );
    }

    return nullptr;
}

}
}
