// Copyright (c) 2019 Cranium Software

#ifndef WRAPPED_PASS_STATEMENT_H
#define WRAPPED_PASS_STATEMENT_H

#include "../../WalkStatement.h"

namespace CP2
{

class ASTNode;

namespace Compiler
{

class PassStatement;

class WrappedPassStatement
: public WalkStatement
{

public:

    WrappedPassStatement(
        PassStatement* const pxPassStatement );
    ~WrappedPassStatement();

    void Execute( Context& xContext ) override;
    WalkStatement* Clone() const override;

    static WalkStatement* Create( const ASTNode* const pxAST );

private:

    PassStatement* mpxPassStatement;

};

}
}

#endif
