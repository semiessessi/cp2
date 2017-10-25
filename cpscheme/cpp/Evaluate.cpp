// Copyright (c) 2017 Cranium Software

#include "Evaluate.h"

#include "../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Scheme
{

void RecursiveASTPrint( ASTNode* const pxNode )
{
	if( pxNode == nullptr )
	{
		puts( "(null)" );
	}
	else if( pxNode->IsTerminal() )
	{
		if( pxNode->IsValued() )
		{
			printf( "%s", pxNode->GetTokenValue().c_str() );
		}
		else
		{
			printf( "%s", pxNode->GetTokenName() );
		}
	}
	else
	{
		for( int i = 0; i < pxNode->GetChildCount(); ++i )
		{
			RecursiveASTPrint( pxNode->GetChild( i ) );
			if( i < ( pxNode->GetChildCount() - 1 ) )
			{
				printf( " " );
			}
		}
	}
}

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

		case ER_AST:
		{
			RecursiveASTPrint( mpxASTValue );
			puts( "" );
			break;
		}

		case ER_PROCEDURE:
		{
			if( mpxASTValue == nullptr )
			{
				printf( "Internal procedure: %s\n", mxStringValue.c_str() );
			}
			else
			{
				printf( "Procedure: %s\n", mxStringValue.c_str() );

				printf( "Parameters: ( " );

				for( const std::string& xName : maxParameterNames )
				{
					printf( "%s ", xName.c_str() );
				}
				
				puts( ")" );

				puts( "Code:" );
				RecursiveASTPrint( mpxASTValue );
				puts( "" );
			}
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

		case ER_AST:
		{
			return mpxASTValue == nullptr;
		}

		case ER_PROCEDURE:
		{
			return false;
		}

		default:
		{
			break;
		}
	}

	return false;
}

EvaluationResult EvaluationResult::Call(
	const std::vector< EvaluationResult >& xParameters, Environment& xEnvironment )
{
	if( meType == ER_PROCEDURE )
	{
		if( mpxASTValue != nullptr )
		{
			Environment xNewEnvironment( xEnvironment );

			// add parameters into the new environment
			for( int i = 0; i < static_cast< int >( xParameters.size() ); ++i )
			{
				xEnvironment.GetVariable( maxParameterNames[ i ].c_str() ) = xParameters[ i ];
			}

			// evaluate with the new environment
			return Evaluate( mpxASTValue, xNewEnvironment );
		}
		else if( mpfnFunction0 )
		{
			return mpfnFunction0();
		}
		else if( mpfnFunction1 )
		{
			if( xParameters.size() >= 1 )
			{
				return mpfnFunction1( xParameters[ 0 ] );
			}
			else
			{
				// SE - TODO: handle error case
			}
		}
		else if( mpfnFunction2 )
		{
			if( xParameters.size() >= 2 )
			{
				return mpfnFunction2( xParameters[ 0 ], xParameters[ 1 ] );
			}
			else
			{
				// SE - TODO: handle error case
			}
		}
	}
	else
	{
		// SE - TODO: handle error case
	}

	return EvaluationResult();
}

int EvaluationResult::GetParameterCount() const
{
	if( meType == ER_PROCEDURE )
	{
		if( mpxASTValue != nullptr )
		{
			return static_cast< int >( maxParameterNames.size() );
		}
		else if( mpfnFunction1 )
		{
			return 1;
		}
		else if( mpfnFunction2 )
		{
			return 2;
		}
	}

	return 0;
}

EvaluationResult Evaluate( ASTNode* const pxASTValue, Environment& xEnvironment )
{
	EvaluationResult xReturnValue;

	if( pxASTValue == nullptr )
	{
		return xReturnValue;
	}

	if( pxASTValue->GetChildCount() == 1 )
	{
		if( std::string( "<identifier>" ) == pxASTValue->GetChild( 0 )->GetTokenName() )
		{
			// variable lookup
			return xEnvironment.GetVariable( pxASTValue->GetChild( 0 )->GetTokenValue().c_str() );
		}
		else if( std::string( "<integer>" ) == pxASTValue->GetTokenName() )
		{
			return EvaluationResult( atoi( pxASTValue->GetChild( 0 )->GetTokenValue().c_str() ) );
		}
	}
	else if( pxASTValue->GetChildCount() > 1 )
	{
		// we have something more.
		// SE - TODO: check for "(" ...
		if( std::string( ")" ) == pxASTValue->GetChild( 1 )->GetTokenName() )
		{
			// empty list?
		}
		else if( std::string( "define" ) == pxASTValue->GetChild( 1 )->GetTokenName() )
		{
			EvaluationResult& xVariable = xEnvironment.GetVariable(
				pxASTValue->GetChild( 2 )->GetTokenValue().c_str() );
			xVariable = Evaluate( pxASTValue->GetChild( 3 ), xEnvironment );
			return xVariable;
		}
		else if( std::string( "if" ) == pxASTValue->GetChild( 1 )->GetTokenName() )
		{
			EvaluationResult xResult = Evaluate( pxASTValue->GetChild( 2 ), xEnvironment );
			if( xResult.IsEquivalentToFalse() )
			{
				return Evaluate( pxASTValue->GetChild( 4 ), xEnvironment );
			}
			else
			{
				return Evaluate( pxASTValue->GetChild( 3 ), xEnvironment );
			}
		}
		// SE - TODO: ... is this good?
		else if( std::string( "set!" ) == pxASTValue->GetChild( 1 )->GetTokenName() )
		{
			if( xEnvironment.VariableIsDefined( pxASTValue->GetChild( 2 )->GetTokenValue().c_str() ) )
			{
				EvaluationResult& xVariable = xEnvironment.GetVariable(
					pxASTValue->GetChild( 2 )->GetTokenValue().c_str() );
				xVariable = Evaluate( pxASTValue->GetChild( 3 ), xEnvironment );
				return xVariable;
			}
			else
			{
				// SE - TODO: complain?!?
				return xReturnValue;
			}
		}
		else
		{
			// it better be a procedure...
			const std::string& xProcedureName = pxASTValue->GetChild( 1 )->GetTokenValue();
			if( xEnvironment.VariableIsDefined( xProcedureName.c_str() ) )
			{
				EvaluationResult& xProcedure =
					xEnvironment.GetVariable( xProcedureName.c_str() );
				if( xProcedure.GetType() == EvaluationResult::ER_PROCEDURE )
				{
					// SE - TODO: safety here.

					// evaluate parameters...
					const int iParameterCount = xProcedure.GetParameterCount();
					std::vector< EvaluationResult > xParameters;
					for( int i = 0; i < iParameterCount; ++i )
					{
						xParameters.push_back(
							Evaluate( pxASTValue->GetChild( 2 + i ), xEnvironment ) );
					}

					return xProcedure.Call( xParameters );
				}
				else
				{
					// SE - TODO: handle error case
				}
			}
			else
			{
				// SE - TODO: handle error case
			}
		}
	}

	// if nothing caught us? just return the ast :/

	xReturnValue = EvaluationResult( pxASTValue );

	return xReturnValue;
}

}
}
