// Copyright (c) 2019 Cranium Software

#ifndef OUTPUT_H
#define OUTPUT_H

#include "../PassStatement.h"

#include <vector>

namespace CP2
{

namespace Compiler
{

class Output
: public PassStatement
{

public:

    Output( const std::string& xPath );
    ~Output();

    void Execute() override;
    PassStatement* Clone() const override;

    void GetRequiredPaths( std::vector< OutputFile >& xFiles ) const override;

    static PassStatement* Create( const ASTNode* const pxAST );

private:

    std::string mxPath;
    bool mbBinary;

};

}
}

#endif
