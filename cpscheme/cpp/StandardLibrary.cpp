// Copyright (c) 2017 Cranium Software

#include "StandardLibrary.h"

#include "Evaluate.h"

namespace CP2
{
namespace Scheme
{

EvaluationResult Addition( EvaluationResult xA, EvaluationResult xB )
{
	if( xA.GetType() == xB.GetType() )
	{
		switch( xA.GetType() )
		{
			case EvaluationResult::ER_INT:
			{
				return EvaluationResult( xA.GetIntValue() + xB.GetIntValue() );
			}

			case EvaluationResult::ER_FLOAT:
			{
				return EvaluationResult( xA.GetFloatValue() + xB.GetFloatValue() );
			}

			case EvaluationResult::ER_STRING:
			{
				return EvaluationResult( xA.GetStringValue() + xB.GetStringValue() );
			}

			default:
			{
				return EvaluationResult();
			}
		}
	}

	// allow adding ints to floats
	if( ( xA.GetType() == EvaluationResult::ER_INT ) && ( xB.GetType() == EvaluationResult::ER_FLOAT ) )
	{
		return EvaluationResult( static_cast< float >( xA.GetIntValue() ) + xB.GetFloatValue() );
	}
	else if( ( xB.GetType() == EvaluationResult::ER_INT ) && ( xA.GetType() == EvaluationResult::ER_FLOAT ) )
	{
		return EvaluationResult( xA.GetFloatValue() + static_cast< float >( xB.GetIntValue() ) );
	}

	// SE - TODO: allow adding ints or floats to strings

	return EvaluationResult();
}

EvaluationResult Subtraction( EvaluationResult xA, EvaluationResult xB )
{
	if( xA.GetType() == xB.GetType() )
	{
		switch( xA.GetType() )
		{
			case EvaluationResult::ER_INT:
			{
				return EvaluationResult( xA.GetIntValue() - xB.GetIntValue() );
			}

			case EvaluationResult::ER_FLOAT:
			{
				return EvaluationResult( xA.GetFloatValue() - xB.GetFloatValue() );
			}

			default:
			{
				return EvaluationResult();
			}
		}
	}

	// allow ints and floats together
	if( ( xA.GetType() == EvaluationResult::ER_INT ) && ( xB.GetType() == EvaluationResult::ER_FLOAT ) )
	{
		return EvaluationResult( static_cast< float >( xA.GetIntValue() ) - xB.GetFloatValue() );
	}
	else if( ( xB.GetType() == EvaluationResult::ER_INT ) && ( xA.GetType() == EvaluationResult::ER_FLOAT ) )
	{
		return EvaluationResult( xA.GetFloatValue() - static_cast< float >( xB.GetIntValue() ) );
	}

	return EvaluationResult();
}

EvaluationResult Multiplication( EvaluationResult xA, EvaluationResult xB )
{
	if( xA.GetType() == xB.GetType() )
	{
		switch( xA.GetType() )
		{
			case EvaluationResult::ER_INT:
			{
				return EvaluationResult( xA.GetIntValue() * xB.GetIntValue() );
			}

			case EvaluationResult::ER_FLOAT:
			{
				return EvaluationResult( xA.GetFloatValue() * xB.GetFloatValue() );
			}

			default:
			{
				return EvaluationResult();
			}
		}
	}

	// allow ints and floats together
	if( ( xA.GetType() == EvaluationResult::ER_INT ) && ( xB.GetType() == EvaluationResult::ER_FLOAT ) )
	{
		return EvaluationResult( static_cast< float >( xA.GetIntValue() ) * xB.GetFloatValue() );
	}
	else if( ( xB.GetType() == EvaluationResult::ER_INT ) && ( xA.GetType() == EvaluationResult::ER_FLOAT ) )
	{
		return EvaluationResult( xA.GetFloatValue() * static_cast< float >( xB.GetIntValue() ) );
	}

	return EvaluationResult();
}

}
}
