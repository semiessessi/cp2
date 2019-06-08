// Copyright (c) 2019 Cranium Software

#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <unordered_map>

namespace CP2
{

namespace Compiler
{

class Pass;
class Variable;

class Context
{
public:

    ~Context();

    Context CreateChild();
    static Context CreateForPass( const Pass& xPass );

    void SetCurrentFile( const std::string& xPath );

    Variable* GetVariable( const std::string& xName );
    const Variable* GetVariable( const std::string& xName ) const;

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
};

}
}

#endif
