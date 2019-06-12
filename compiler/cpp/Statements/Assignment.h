// Copyright (c) 2019 Cranium Software

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "../PassStatement.h"

#include <vector>

namespace CP2
{

class ASTNode;

namespace Compiler
{

class Assignment
: public PassStatement
{

public:

    Assignment(
        const std::string& xVariableName,
        const ASTNode* const pxExpression );

    void Execute( Context& xContext ) override;
    PassStatement* Clone() const override;

    static PassStatement* Create( const ASTNode* const pxAST );

private:

    std::string mxVariableName;
    const ASTNode* mpxExpression;

};

}
}

#endif
