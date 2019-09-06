// Copyright (c) 2019 Cranium Software

#ifndef WALK_H
#define WALK_H

#include "../PassStatement.h"

namespace CP2
{

class ASTNode;

namespace Compiler
{

class WalkStatement;

class Walk
: public PassStatement
{

public:

    Walk(
        const ASTNode* const pxExpression,
        const std::string& xIdentifier,
        const ASTNode* const pxScope );
    ~Walk();

    void Execute( Context& xContext ) override;
    PassStatement* Clone() const override;

    static PassStatement* Create( const ASTNode* const pxAST );

private:

    const ASTNode* mpxParseExpression;
    const ASTNode* mpxStatements;
    std::string mxName;
    WalkStatement* mpxScope;

};

}
}

#endif
