// Copyright (c) 2019 Cranium Software

#ifndef CONTEXT_H
#define CONTEXT_H

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

class Pass;
class Passes;
class Variable;

struct ParseWalkState
{
    bool bStopped;
    bool bSkipped;
};

class Context
{
public:

    ~Context();

    Context CreateChild();
    static Context CreateForPass(
        const Pass& xPass,
        const Parser::Grammar& xGrammar,
        const Passes& xPasses );

    void* GetCurrentFile() { return mpCurrentFile; }
    void SetCurrentFile( const std::string& xPath );

    Variable* GetVariable( const std::string& xName );
    const Variable* GetVariable( const std::string& xName ) const;

    void UpdateVariable( const std::string& xName, const std::string& xValue, const bool bLocal = false );
    void UpdateVariable( const std::string& xName, const Variable* const pxVariable, const bool bLocal = false );

    const Parser::Grammar* GetGrammar() const { return mpxGrammar; }
    const Passes* GetPasses() const { return mpxPasses; }

    const ParseWalkState& GetWalkState() const { return mxPassWalkState; }

    void WalkBreak() { mxPassWalkState.bStopped = true; }
    void WalkContinue() { mxPassWalkState.bSkipped = true; }
private:

    Context();
    Context( const Context& xOther );
    Context( const Context* const pxOther );
    Context& operator=( const Context& xOther );

    void* GetFileFromPath( const std::string& xPath );

    std::unordered_map< std::string, Variable* > mxVariables;
    std::unordered_map< std::string, void* > mxFiles;
    std::string mxCurrentPath;
    void* mpCurrentFile;
    Context* mpxParentContext;
    const Parser::Grammar* mpxGrammar;
    const Passes* mpxPasses;

    ParseWalkState mxPassWalkState;
};

}
}

#endif
