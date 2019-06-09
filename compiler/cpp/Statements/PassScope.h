// Copyright (c) 2019 Cranium Software

#ifndef PASS_SCOPE_H
#define PASS_SCOPE_H

#include "../PassStatement.h"

#include <vector>

namespace CP2
{

namespace Compiler
{

class Context;

class PassScope
: public PassStatement
{

public:

    PassScope( const PassScope* const pxOther );
    ~PassScope();

    void Execute( Context& xContext ) override;
    PassStatement* Clone() const override;

    static PassStatement* Create( const ASTNode* const pxAST );

private:

    std::vector< PassStatement* > maxStatements;

};

}
}

#endif
