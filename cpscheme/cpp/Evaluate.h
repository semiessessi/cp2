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

	EvaluationResult( const EvaluationResult& xOther )
	: meType( xOther.meType )
	, miIntValue( xOther.miIntValue )
	, mfFloatValue( xOther.mfFloatValue )
	, mxStringValue( xOther.mxStringValue )
	, mpxASTValue( xOther.mpxASTValue )
	{

	}

	void Output() const;

	bool IsEquivalentToFalse() const;

private:

	Type meType;
	int miIntValue;
	float mfFloatValue;
	std::string mxStringValue;
	ASTNode* mpxASTValue;
};

EvaluationResult Evaluate(
	ASTNode* const mpxASTValue,
	Environment& xEnvironment = Environment::GlobalEnvironment() );

}
}

#endif
