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

private:

    Context();
    Context( const Context& );
    Context( Context&& ) {}

    std::unordered_map< std::string, Variable* > mxVariables;
    std::unordered_map< std::string, void* > mxFiles;
    std::string mxCurrentPath;
    void* mpCurrentFile;
    Context* mpxParentContext;
};

}
}

#endif
