// Copyright (c) 2017 Cranium Software

#include "ASTNode.h"

namespace CP2
{

std::string ASTNode::DebugPrint() const
{
	std::string xReturnValue;
	DebugPrintRecursive( xReturnValue );
	return xReturnValue;
}

ASTNode* ASTNode::DuplicateAndAddChild( const ASTNode* const pxNode, ASTNode* const pxChild )
{
	return new ASTNode( pxNode, pxChild );
}

ASTNode* ASTNode::Duplicate( const ASTNode* const pxNode )
{
	return new ASTNode( pxNode );
}

std::vector< ASTNode* > ASTNode::Append( const std::vector< ASTNode* >& xA, ASTNode* const pxB )
{
	std::vector< ASTNode* > xReturnValue = xA;
	xReturnValue.push_back( pxB );
	return xReturnValue;
}

void ASTNode::DebugPrintRecursive( std::string& xWorkingString ) const
{
	xWorkingString += mxProductionName;
	xWorkingString += "( ";

	if( ( mapxChildren.size() == 0 ) )
	{
		xWorkingString += mxToken.IsValued() ? mxToken.GetValue() : mxToken.GetName();
	}

	for( size_t i = 0; i < mapxChildren.size(); ++i )
	{
		if( i != 0 )
		{
			xWorkingString += ", ";
		}
		mapxChildren[ i ]->DebugPrintRecursive( xWorkingString );
	}

	xWorkingString += " )";
}

std::vector< ASTNode* > ASTNode::DeepDuplicate( const std::vector< ASTNode* >& xA )
{
	std::vector< ASTNode* > xReturnValue;

	for( ASTNode* pxNode : xA )
	{
		xReturnValue.push_back( new ASTNode( pxNode ) );
	}

	return xReturnValue;
}

ASTNode::ASTNode( const ASTNode* const pxTemplate, ASTNode* const pxChild )
: mapxChildren( Append( DeepDuplicate( pxTemplate->mapxChildren ), pxChild ) )
, mxProductionName( pxTemplate->mxProductionName )
, mxToken( pxTemplate->mxToken )
, miCursor( pxTemplate->miCursor )
{

}

ASTNode::ASTNode( const ASTNode* const pxTemplate )
: mapxChildren( DeepDuplicate( pxTemplate->mapxChildren ) )
, mxProductionName( pxTemplate->mxProductionName )
, mxToken( pxTemplate->mxToken )
, miCursor( pxTemplate->miCursor )
{

}

}
