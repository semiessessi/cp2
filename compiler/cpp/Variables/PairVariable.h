// Copyright (c) 2019 Cranium Software

#ifndef PAIR_VARIABLE_H
#define PAIR_VARIABLE_H

#include "Variable.h"

#include <string>
#include <vector>

namespace CP2
{

namespace Compiler
{

class PairVariable
: public Variable
{
public:

    PairVariable(
        const std::string& xName, 
        const Variable* const pxFirst,
        const Variable* const pxLast );
    ~PairVariable();

    Variable* Clone() const override { return new PairVariable( GetName(), mpxFirst, mpxLast ); }

    Variable* GetIndexed( const int iIndex ) const override { return iIndex ? mpxLast : mpxFirst; }
    int GetIndexedCount() const override { return 2; }

    std::string GetStartValue() const override { return GetIndexed( 0 )->GetValue(); }
    std::string GetEndValue() const override { return GetIndexed( 1 )->GetValue(); }

private:

    Variable* mpxFirst;
    Variable* mpxLast;

};

}
}

#endif
