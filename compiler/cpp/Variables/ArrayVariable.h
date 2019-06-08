// Copyright (c) 2019 Cranium Software

#ifndef ARRAY_VARIABLE_H
#define ARRAY_VARIABLE_H

#include "Variable.h"

#include <string>
#include <vector>

namespace CP2
{

namespace Compiler
{

class ArrayVariable
: public Variable
{
public:

    ArrayVariable( const std::string& xName, const std::vector< Variable* >& xArray );

    Variable* Clone() const override { return new ArrayVariable( GetName(), mxContent ); }

    Variable* GetIndexed( const int iIndex ) const override { return mxContent[ iIndex ]; }
    int GetIndexedCount() const override { return static_cast< int >( mxContent.size() ); }
private:

    std::vector< Variable* > mxContent;

};

}
}

#endif
