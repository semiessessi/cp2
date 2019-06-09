// Copyright (c) 2019 Cranium Software

#ifndef CONDITIONAL_H
#define CONDITIONAL_H

#include "../PassStatement.h"

namespace CP2
{

class ASTNode;

namespace Compiler
{

class Conditional
: public PassStatement
{

public:

    Conditional(
        const ASTNode* const pxExpression,
        const PassStatement* const pxIf,
        const PassStatement* const pxElse = nullptr );
    ~Conditional();

    void Execute( Context& xContext ) override;
    PassStatement* Clone() const override;

    static PassStatement* Create( const ASTNode* const pxAST );

private:

    const ASTNode* mpxBooleanExpression;
    PassStatement* mpxIf;
    PassStatement* mpxElse;

};

}
}

#endif
