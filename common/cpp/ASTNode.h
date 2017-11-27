// Copyright (c) 2017 Cranium Software

#ifndef ASTNODE_H
#define ASTNODE_H

#include "Pool.h"
#include "Token.h"

#include <string>
#include <string_view>
#include <vector>

namespace CP2
{

struct ParseError
{
	int iNumber;
	//std::string_view xExpected;
};

class ASTNode
: public PoolAllocated< ASTNode >
{
	
public:

	// error node
	ASTNode( const int iCursor,
		const Token& xToken,
		const std::string_view& xProductionName,
		const int iErrorNumber,
		const std::string_view& xExpected )
	: mxProductionName( xProductionName )
	, mxToken( xToken )
	, miCursor( iCursor )
	{
		ParseError xParseError =
		{
			iErrorNumber,
			//xExpected,
		};

		maxErrors.push_back( xParseError );
	}

	ASTNode( const int iCursor,
		const Token& xToken,
		const std::string_view& xProductionName,
		const std::vector< ASTNode* >& apxChildren = std::vector< ASTNode* >() )
	: mapxChildren( apxChildren )
	, mxProductionName( xProductionName )
	, mxToken( xToken )
	, miCursor( iCursor )
	{

	}

	~ASTNode()
	{
#ifdef _DEBUG
		// SE - NOTE: this is because some MS debug code
		// gets tripped here otherwise whilst i was debugging a problem
		if( mapxChildren.size() == 0 )
		{
			return;
		}
#endif
		for( ASTNode* pxChild : mapxChildren )
		{
			delete pxChild;
		}
	}

	std::string DebugPrint() const;

	bool IsTerminal() const { return mapxChildren.size() == 0; }
	bool IsNonTerminal() const { return !IsTerminal(); }

	int GetCursor() const { return miCursor; }
	const std::string& GetProductionName() const { return mxProductionName; }
	const char* GetFilename() const { return mxToken.GetFilename(); }
	int GetLine() const { return mxToken.GetLine(); }
	int GetColumn() const { return mxToken.GetColumn(); }
	const std::string& GetTokenValue() const { return mxToken.GetValue(); }
	const char* GetTokenName() const { return mxToken.GetName(); }

	static ASTNode* DuplicateAndAddChild( const ASTNode* const pxNode, ASTNode* const pxChild );
	static ASTNode* Duplicate( const ASTNode* const pxNode );

	int GetChildCount() const { return static_cast< int >( mapxChildren.size() ); }
	ASTNode* GetChild( const int i ) const { return mapxChildren[ i ]; }

	bool IsValued() const { return mxToken.IsValued(); }
	bool IsErrored() const
	{
		// SE - TODO: this shouldn't need to be recursive (!)
		if( !maxErrors.empty() )
		{
			return true;
		}

		for( const ASTNode* const pxChild : mapxChildren )
		{
			if( pxChild->IsErrored() )
			{
				return true;
			}
		}

		return false;
	}

	void VisitTopDownLeftmost( class ASTVisitor& xVisitor );
	void VisitBottomUpLeftmost( class ASTVisitor& xVisitor );

private:

	void DebugPrintRecursive( std::string& xWorkingString ) const;

	static std::vector< ASTNode* > Append( const std::vector< ASTNode* >& xA, ASTNode* const pxB );
	static std::vector< ASTNode* > DeepDuplicate( const std::vector< ASTNode* >& xA );

	ASTNode( const ASTNode* const pxTemplate, ASTNode* const pxChild );
	ASTNode( const ASTNode* const pxTemplate );

	std::vector< ASTNode* > mapxChildren;
	std::vector< ParseError > maxErrors;
	std::string mxProductionName;
	Token mxToken;
	int miCursor;

};

}

#endif
