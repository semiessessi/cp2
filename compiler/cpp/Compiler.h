// Copyright (c) 2019 Cranium Software

#ifndef COMPILER_H
#define COMPILER_H

namespace CP2
{

class ASTNode;

namespace Compiler
{

class Passes;

// SE - TODO: return value etc?
void Compile( const ASTNode* const pxAST, const Passes& xPasses );

}
}

#endif
