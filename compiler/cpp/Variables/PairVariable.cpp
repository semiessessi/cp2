// Copyright (c) 2019 Cranium Software

#include "PairVariable.h"

namespace CP2
{
namespace Compiler
{

PairVariable::PairVariable(
    const std::string& xName,
    const Variable* const pxFirst,
    const Variable* const pxLast )
: Variable( xName )
, mpxFirst( pxFirst->Clone() )
, mpxLast( pxLast->Clone() )
{
}

PairVariable::~PairVariable()
{
    delete mpxFirst;
    delete mpxLast;
}

}
}
