// Copyright (c) 2017 Cranium Software

#ifndef CPS_STDLIB_H
#define CPS_STDLIB_H

#include "Evaluate.h"

namespace CP2
{
namespace Scheme
{

EvaluationResult Addition( EvaluationResult xA, EvaluationResult xB );
EvaluationResult Subtraction( EvaluationResult xA, EvaluationResult xB );
EvaluationResult Multiplication( EvaluationResult xA, EvaluationResult xB );

}
}

#endif
