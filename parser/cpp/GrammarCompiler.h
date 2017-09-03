#ifndef GRAMMAR_COMPILER_H
#define GRAMMAR_COMPILER_H

namespace CP2
{

class ASTNode;

namespace Parser
{

class Grammar;

Grammar CompileGrammar( ASTNode* const pxAST );

}
}

#endif
