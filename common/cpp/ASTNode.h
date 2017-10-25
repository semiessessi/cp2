// Copyright (c) 2017 Cranium Software

#ifndef ASTNODE_H
#define ASTNODE_H

#include "Pool.h"
#include "Token.h"

#include <string>
#include <vector>

namespace CP2
{

class ASTNode
: public PoolAllocated< ASTNode >
{
	
public:

	ASTNode( const int iCursor,
		const Token& xToken,
		const std::string& xProductionName,
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

private:

	void DebugPrintRecursive( std::string& xWorkingString ) const;

	static std::vector< ASTNode* > Append( const std::vector< ASTNode* >& xA, ASTNode* const pxB );
	static std::vector< ASTNode* > DeepDuplicate( const std::vector< ASTNode* >& xA );

	ASTNode( const ASTNode* const pxTemplate, ASTNode* const pxChild );
	ASTNode( const ASTNode* const pxTemplate );

	std::vector< ASTNode* > mapxChildren;
	std::string mxProductionName;
	Token mxToken;
	int miCursor;

};

}

#endif
