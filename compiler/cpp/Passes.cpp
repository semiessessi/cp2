// Copyright (c) 2019 Cranium Software

#include "Passes.h"


namespace CP2
{
namespace Compiler
{

void Passes::Execute( const Parser::Grammar& xGrammar )
{
    std::vector< std::string > axCompletedPasses;
    // run all passes where all the requisites were run already
    // repeat until done (fails if dependencies are cyclic and other madness...)
    // so for safety max to 32 iterations
    const int kiIterationCount = 32;
    for( int iLimiter = 0; iLimiter < kiIterationCount; ++iLimiter )
    {
        bool bDone = true;
        for( auto xPassPair : mxPasses )
        {
            std::vector< std::string > axRequiredPasses;
            xPassPair.second.GetRequiredPasses( axRequiredPasses );
            bool bMissing = false;
            for( const std::string& xPassName : axRequiredPasses )
            {
                if( std::find( axCompletedPasses.begin(), axCompletedPasses.end(), xPassName )
                    == axCompletedPasses.end() )
                {
                    bDone = false;
                    bMissing = true;
                    break;
                }
            }

            if( bMissing == false )
            {
                xPassPair.second.Execute( xGrammar );
                axCompletedPasses.push_back( xPassPair.first );
            }
        }

        if( bDone )
        {
            break;
        }
    }
}

}
}