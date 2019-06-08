// Copyright (c) 2019 Cranium Software

#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

namespace CP2
{
namespace Compiler
{

class Variable
{

public:

    Variable( const std::string& xName );
    virtual ~Variable() {}

    // cloning is important enough to be virtual and pure
    virtual Variable* Clone() const = 0;

    virtual std::string GetValue() const;
    virtual std::string GetNameValue() const { return GetName(); }
    virtual Variable* GetIndexed( const int iIndex ) const;
    const std::string& GetName() const { return mxName; }

private:

    std::string mxName;

};

}
}

#endif
