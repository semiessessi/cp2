// Copyright (c) 2019 Cranium Software

#ifndef OUTPUT_H
#define OUTPUT_H

#include "../PassStatement.h"

#include <vector>

namespace CP2
{

class ASTNode;

namespace Compiler
{

class Output
: public PassStatement
{

public:

    Output( const std::string& xPath );
    Output( const ASTNode* const pxExpression );
    ~Output();

    void Execute( Context& xContext ) override;
    PassStatement* Clone() const override;

    void GetRequiredPaths( std::vector< OutputFile >& xFiles ) const override;

    static PassStatement* Create( const ASTNode* const pxAST );

private:

    std::string mxPath;
    const ASTNode* mpxStringExpression;
    bool mbBinary;

};

}
}

#endif
