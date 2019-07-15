// Copyright (c) 2019 Cranium Software

#ifndef PASSES_H
#define PASSES_H

#include "Pass.h"

#include <string>
#include <unordered_map>

namespace CP2
{
namespace Parser
{
class Grammar;
}
namespace Compiler
{

class Passes
{

public:

    Pass& AddPass( const std::string& xName )
    {
        // SE - TODO: warn if exists?
        return mxPasses[ xName ];
    }

    void Merge( const Passes& xOther )
    {
        // SE - TODO: warn if replacing?
        for( const std::pair< const std::string, Pass >& xMapping : xOther.mxPasses )
        {
            mxPasses[ xMapping.first ] = xMapping.second;
        }
    }

    void Execute( const Parser::Grammar& xGrammar );

    const Pass& GetPass( const int iIndex ) const;
    int GetPassCount() const { return static_cast< int >( mxPasses.size() ); }

private:

    std::unordered_map< std::string, Pass > mxPasses;

};

}
}

#endif
