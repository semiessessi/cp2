// Copyright (c) 2019 Cranium Software

#ifndef WALK_STATEMENT_H
#define WALK_STATEMENT_H

#include "OutputFile.h"

#include <string>
#include <vector>

namespace CP2
{

class ASTNode;

namespace Compiler
{

class Context;
class PassStatement;
class Variable;

class WalkStatement
{

public:
    
    virtual ~WalkStatement() {}
    
    // cloning is important enough to be virtual and pure
    virtual WalkStatement* Clone() const = 0;

    // other stuff is optional.
    virtual void Execute( Context& xContext ) { Execute(); }
    virtual void Execute() {}
    virtual void GetRequiredPaths( std::vector< OutputFile >& xFiles ) const {}

    static WalkStatement* Create( const ASTNode* const pxAST );

};

}
}

#endif
