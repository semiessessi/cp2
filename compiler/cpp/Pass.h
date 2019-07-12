// Copyright (c) 2019 Cranium Software

#ifndef PASS_H
#define PASS_H

#include "OutputFile.h"

#include <vector>
#include <string>

namespace CP2
{

class ASTNode;

namespace Parser
{
class Grammar;
}

namespace Compiler
{

class Passes;
class PassScope;

class Pass
{

public:

    Pass( const std::string& xName = "<unknown-pass>" );
    Pass( const Pass& xPass );
    ~Pass();

    Pass& operator =( const Pass& xPass );

    void SetName( const std::string& xName )
    {
        mxName = xName;
    }

    void SetSwitch( const bool bSwitchValue )
    {
        mbIsSwitch = bSwitchValue;
    }

    void AddRequiredPass( const std::string& xPassName );

    void Compile( const ASTNode* const );

    void GetRequiredPaths( std::vector< OutputFile >& xFiles );
    void GetRequiredPasses( std::vector< std::string >& xPassNames );

    void Execute( const Parser::Grammar& xGrammar, const Passes& xPasses );

    const std::string& GetName() const { return mxName; }

private:

    void CreateRequiredPaths( const std::vector< OutputFile >& xFile );

    bool mbIsSwitch;
    PassScope* mpxStatements;
    std::vector< std::string > maxRequiredPasses;
    std::string mxName;

};

}
}

#endif
