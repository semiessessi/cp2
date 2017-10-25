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

		default:
		{
			break;
		}
	}

	return false;
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
		// SE - TODO: ...
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
	}

	// if nothing caught us? just return the ast :/

	xReturnValue = EvaluationResult( pxASTValue );

	return xReturnValue;
}

}
}
