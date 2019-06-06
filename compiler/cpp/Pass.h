// Copyright (c) 2019 Cranium Software

#ifndef PASS_H
#define PASS_H

#include "OutputFile.h"

#include <vector>
#include <string>

namespace CP2
{

class ASTNode;

namespace Compiler
{

class PassScope;

class Pass
{

public:

    Pass();
    Pass( const Pass& xPass );
    ~Pass();

    Pass& operator =( const Pass& xPass );

    void SetSwitch( const bool bSwitchValue )
    {
        mbIsSwitch = bSwitchValue;
    }

    void Compile( const ASTNode* const );

    void GetRequiredPaths( std::vector< OutputFile >& xFiles );

    void Execute();

private:

    bool mbIsSwitch;
    PassScope* mpxStatements;

};

}
}

#endif
