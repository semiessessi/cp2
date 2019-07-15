// Copyright (c) 2019 Cranium Software

#ifndef PARSE_VARIABLE_H
#define PARSE_VARIABLE_H

#include "Variable.h"

#include <string>

namespace CP2
{

class ASTNode;

namespace Compiler
{

class ParseVariable
: public Variable
{
public:
    ParseVariable(
        const std::string& xName,
        const ASTNode* const pxNode );
    ~ParseVariable();

    Variable* Clone() const override { return new ParseVariable( GetName(), mpxNode ); }

protected:

    const ASTNode* mpxNode;
};

}
}

#endif
