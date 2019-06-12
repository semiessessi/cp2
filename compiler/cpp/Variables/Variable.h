// Copyright (c) 2019 Cranium Software

#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <vector>

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
    virtual std::string GetStartValue() const { return GetValue(); }
    virtual std::string GetEndValue() const { return GetValue(); }

    virtual Variable* GetIndexed( const int iIndex ) const;
    virtual int GetIndexedCount() const;

    virtual bool HasNames() const { return false; }

    virtual bool IsOptionalName() const { return false; }
    virtual bool IsMultipleName() const { return false; }
    virtual bool IsNonEmptyName() const { return false; }

    const std::string& GetName() const { return mxName; }
    virtual std::string GetInputName() const;

    virtual Variable* GetKeywords() const;
    virtual Variable* GetIdentifiers() const;
    virtual Variable* GetStrings() const;
    virtual Variable* GetOperators() const;
    virtual Variable* GetTerminators() const;
    virtual Variable* GetSeparators() const;
    
    virtual Variable* GetLexemes() const;
    virtual Variable* GetQuotes() const;
    virtual Variable* GetLineComments() const;
    virtual Variable* GetBlockComments() const;

    static void DeleteArray( const std::vector< Variable* >& xVariables );

protected:

    static std::string InputNameFromOutputName( const std::string& xOutputName );

private:

    std::string mxName;
};

}
}

#endif
