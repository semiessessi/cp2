// Copyright (c) 2019 Cranium Software

#ifndef PASS_STATEMENT_H
#define PASS_STATEMENT_H

#include "OutputFile.h"

#include <string>
#include <vector>

namespace CP2
{

class ASTNode;

namespace Compiler
{

class Context;
class Variable;

class PassStatement
{

public:
    
    virtual ~PassStatement() {}
    
    // cloning is important enough to be virtual and pure
    virtual PassStatement* Clone() const = 0;

    // other stuff is optional.
    virtual void Execute( Context& xContext ) { Execute(); }
    virtual void Execute() {}
    virtual void GetRequiredPaths( std::vector< OutputFile >& xFiles ) const {}

    static PassStatement* Create( const ASTNode* const pxAST );

protected:

    static Variable* EvaluateArrayExpression( const ASTNode* const pxAST, const Context& xContext );
    static bool EvaluateBooleanExpression( const ASTNode* const pxAST, const Context& xContext );
    static std::string EvaluateStringExpression( const ASTNode* const pxAST, const Context& xContext );
    static Variable* EvaluateParseExpression( const ASTNode* const pxAST, const Context& xContext );
};

}
}

#endif
