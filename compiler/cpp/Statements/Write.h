// Copyright (c) 2019 Cranium Software

#ifndef WRITE_H
#define WRITE_H

#include "../PassStatement.h"


namespace CP2
{

class ASTNode;

namespace Compiler
{

class Write
: public PassStatement
{

public:

    Write( const ASTNode* const pxExpression );

    void Execute( Context& xContext ) override;
    PassStatement* Clone() const override;

    static PassStatement* Create( const ASTNode* const pxAST );

private:

    const ASTNode* mpxStringExpression;

};

}
}

#endif
