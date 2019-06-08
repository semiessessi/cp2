// Copyright (c) 2019 Cranium Software

#ifndef FOREACH_H
#define FOREACH_H

#include "../PassStatement.h"

namespace CP2
{

class ASTNode;

namespace Compiler
{

class ForEach
: public PassStatement
{

public:

    ForEach(
        const ASTNode* const pxExpression,
        const std::string& xIdentifier,
        const ASTNode* const pxScope );

    void Execute( Context& xContext ) override;
    PassStatement* Clone() const override;

    static PassStatement* Create( const ASTNode* const pxAST );

private:

    const ASTNode* mpxArrayExpression;
    const ASTNode* mpxStatements;
    std::string mxName;
    PassStatement* mpxScope;

};

}
}

#endif
