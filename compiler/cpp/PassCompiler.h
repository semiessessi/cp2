// Copyright (c) 2019 Cranium Software

#ifndef PASS_COMPILER_H
#define PASS_COMPILER_H

#include "Passes.h"

namespace CP2
{

class ASTNode;

namespace Compiler
{

Passes CompilePasses( const ASTNode* const pxAST );

}
}

#endif
