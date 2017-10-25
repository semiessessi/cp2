// Copyright (c) 2017 Cranium Software

#ifndef CPS_EVALUATE_H
#define CPS_EVALUATE_H

#include <string>

#include "Environment.h"

namespace CP2
{

class ASTNode;

namespace Scheme
{

class EvaluationResult
{

public:

	enum Type
	{
		ER_NULL = 0,
		ER_INT,
		ER_BOOL,
		ER_FLOAT,
		ER_STRING,
		ER_AST,
		ER_PROCEDURE,

		ER_COUNT
	};

	EvaluationResult()
	: meType( ER_NULL )
	{

	}

	EvaluationResult( const int iInt )
	: meType( ER_INT )
	, miIntValue( iInt )
	{

	}

	EvaluationResult( const float fFloat )
	: meType( ER_FLOAT )
	, mfFloatValue( fFloat )
	{

	}

	EvaluationResult( const std::string& xString )
	: meType( ER_STRING )
	, mxStringValue( xString )
	{

	}

	EvaluationResult( ASTNode* const pxNode )
	: meType( ER_AST )
	, mpxASTValue( pxNode )
	{

	}

	EvaluationResult(
		const char* const szName,
		const std::vector< std::string > axParameters,
		ASTNode* const pxNode )
	: meType( ER_PROCEDURE )
	, mxStringValue( szName )
	, mpxASTValue( pxNode )
	, maxParameterNames( axParameters )
	{

	}

	EvaluationResult(
		const char* const szName,
		EvaluationResult ( * const pfnFunction0 )() )
	: meType( ER_PROCEDURE )
	, mxStringValue( szName )
	, mpxASTValue( nullptr )
	, mpfnFunction0( pfnFunction0 )
	, mpfnFunction1( nullptr )
	, mpfnFunction2( nullptr )
	{

	}

	EvaluationResult(
		const char* const szName,
		EvaluationResult ( * const pfnFunction1 )( EvaluationResult ) )
	: meType( ER_PROCEDURE )
	, mxStringValue( szName )
	, mpxASTValue( nullptr )
	, mpfnFunction0( nullptr )
	, mpfnFunction1( pfnFunction1 )
	, mpfnFunction2( nullptr )
	{

	}

	EvaluationResult(
		const char* const szName,
		EvaluationResult ( * const pfnFunction2 )( EvaluationResult, EvaluationResult ) )
	: meType( ER_PROCEDURE )
	, mxStringValue( szName )
	, mpxASTValue( nullptr )
	, mpfnFunction0( nullptr )
	, mpfnFunction1( nullptr )
	, mpfnFunction2( pfnFunction2 )
	{

	}

	EvaluationResult( const EvaluationResult& xOther )
	: meType( xOther.meType )
	, miIntValue( xOther.miIntValue )
	, mfFloatValue( xOther.mfFloatValue )
	, mxStringValue( xOther.mxStringValue )
	, mpxASTValue( xOther.mpxASTValue )
	{

	}

	void Output() const;

	Type GetType() const { return meType; }
	bool IsEquivalentToFalse() const;

	int GetIntValue() const { return miIntValue; }
	float GetFloatValue() const { return mfFloatValue; }
	const std::string& GetStringValue() const { return mxStringValue; }

	EvaluationResult Call(
		const std::vector< EvaluationResult >& xParameters,
		Environment& xEnvironment = Environment::GlobalEnvironment() );
	int GetParameterCount() const;

private:

	Type meType;
	int miIntValue;
	float mfFloatValue;
	std::string mxStringValue;
	ASTNode* mpxASTValue;
	EvaluationResult ( *mpfnFunction0 )();
	EvaluationResult ( *mpfnFunction1 )( EvaluationResult );
	EvaluationResult ( *mpfnFunction2 )( EvaluationResult, EvaluationResult );
	std::vector< std::string > maxParameterNames;
};

EvaluationResult Evaluate(
	ASTNode* const mpxASTValue,
	Environment& xEnvironment = Environment::GlobalEnvironment() );

}
}

#endif
