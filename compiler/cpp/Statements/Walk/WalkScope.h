// Copyright (c) 2019 Cranium Software

#ifndef WALK_SCOPE_H
#define WALK_SCOPE_H

#include "../../WalkStatement.h"

#include <vector>

namespace CP2
{

namespace Compiler
{

class Context;

class WalkScope
: public WalkStatement
{

public:

    WalkScope( const WalkScope* const pxOther );
    ~WalkScope();

    void Execute( Context& xContext ) override;
    WalkStatement* Clone() const override;

    static WalkStatement* Create( const ASTNode* const pxAST );

private:

    std::vector< WalkStatement* > maxStatements;

};

}
}

#endif
