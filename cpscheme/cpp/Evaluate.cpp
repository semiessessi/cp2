// Copyright (c) 2017 Cranium Software

#include "Evaluate.h"

#include "../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Scheme
{

void EvaluationResult::Output() const
{
	switch( meType )
	{
		case ER_NULL:
		{
			puts( "(null)" );
			break;
		}

		case ER_INT:
		{
			printf( "%d\n", miIntValue );
			break;
		}

		case ER_STRING:
		{
			puts( mxStringValue.c_str() );
			break;
		}

		default:
		{
			puts( "! Unknown result !" );
			break;
		}
	}
}

bool EvaluationResult::IsEquivalentToFalse() const
{
	switch( meType )
	{
		case ER_NULL:
		{
			return true;
		}

		case ER_INT:
		{
			return miIntValue == 0;
		}

		case ER_STRING:
		{
			return mxStringValue.empty() || ( mxStringValue == "false" );
		}

		default:
		{
			break;
		}
	}

	return false;
}

EvaluationResult Evaluate( const ASTNode* const mpxASTValue, Environment& xEnvironment )
{
	EvaluationResult xReturnValue;

	if( mpxASTValue == nullptr )
	{
		return xReturnValue;
	}

	if( mpxASTValue->GetChildCount() == 1 )
	{
		if( std::string( "<identifier>" ) == mpxASTValue->GetChild( 0 )->GetTokenName() )
		{
			// variable lookup
			return xEnvironment.GetVariable( mpxASTValue->GetChild( 0 )->GetTokenValue().c_str() );
		}
		else if( std::string( "<integer>" ) == mpxASTValue->GetTokenName() )
		{
			return EvaluationResult( atoi( mpxASTValue->GetChild( 0 )->GetTokenValue().c_str() ) );
		}
	}
	else if( mpxASTValue->GetChildCount() > 1 )
	{
		// we have something more.
		if( std::string( ")" ) == mpxASTValue->GetChild( 1 )->GetTokenName() )
		{
			// empty list?
		}
		else if( std::string( "define" ) == mpxASTValue->GetChild( 1 )->GetTokenName() )
		{
			EvaluationResult& xVariable = xEnvironment.GetVariable(
				mpxASTValue->GetChild( 2 )->GetTokenValue().c_str() );
			xVariable = Evaluate( mpxASTValue->GetChild( 3 ), xEnvironment );
			return xVariable;
		}
		else if( std::string( "if" ) == mpxASTValue->GetChild( 1 )->GetTokenName() )
		{
			EvaluationResult xResult = Evaluate( mpxASTValue->GetChild( 2 ), xEnvironment );
			if( xResult.IsEquivalentToFalse() )
			{
				return Evaluate( mpxASTValue->GetChild( 4 ), xEnvironment );
			}
			else
			{
				return Evaluate( mpxASTValue->GetChild( 3 ), xEnvironment );
			}
		}
		// SE - TODO: ...
		else if( std::string( "set!" ) == mpxASTValue->GetChild( 1 )->GetTokenName() )
		{
			if( xEnvironment.VariableIsDefined( mpxASTValue->GetChild( 2 )->GetTokenValue().c_str() ) )
			{
				EvaluationResult& xVariable = xEnvironment.GetVariable(
					mpxASTValue->GetChild( 2 )->GetTokenValue().c_str() );
				xVariable = Evaluate( mpxASTValue->GetChild( 3 ), xEnvironment );
				return xVariable;
			}
			else
			{
				// SE - TODO: complain?!?
				return xReturnValue;
			}
		}
	}

	return xReturnValue;
}

}
}
