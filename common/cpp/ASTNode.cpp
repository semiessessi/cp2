// Copyright (c) 2017 Cranium Software

#include "ASTNode.h"
#include "ASTVisitor.h"

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

void ASTNode::VisitTopDownLeftmost( ASTVisitor& xVisitor )
{
	xVisitor.DoVisit( *this );

	for( ASTNode* const pxChild : mapxChildren )
	{
		pxChild->VisitTopDownLeftmost( xVisitor );
	}
}

void ASTNode::VisitBottomUpLeftmost( ASTVisitor& xVisitor )
{
	for( ASTNode* const pxChild : mapxChildren )
	{
		pxChild->VisitBottomUpLeftmost( xVisitor );
	}

	xVisitor.DoVisit( *this );
}

std::vector< ASTNode* > ASTNode::Append( const std::vector< ASTNode* >& xA, ASTNode* const pxB )
{
	std::vector< ASTNode* > xReturnValue = xA;
	xReturnValue.push_back( pxB );
	return xReturnValue;
}

std::string ASTNode::GetErrorString() const
{
    if( maxErrors.empty() )
    {
        return "Unhandled parser error "
            "(no information - please report this as a bug)";
    }

    std::string xReturnValue = "Unhandled parser error "
        "(no information - please report this as a bug)";

    if( maxErrors.size() == 1 )
    {
        xReturnValue = "error ";
        xReturnValue += maxErrors[ 0 ].iNumber;
        xReturnValue += ": expected ";
        xReturnValue += maxErrors[ 0 ].xExpected.xName;
    }

	return xReturnValue;
}

const Parser::Name& ASTNode::GetExpectedName() const
{
    return maxErrors[ 0 ].xExpected;
}

const Parser::GrammarProduction& ASTNode::GetExpectedProduction() const
{
    return *mpxProduction;
}

void ASTNode::TidyRecursions()
{
    if( mxProductionName.bSubstitution == false )
    {
        // loop over children and tidy or call accordingly
        const size_t iBegin = 0;
        size_t iEnd = 0;
        for( size_t i = 0; i < mapxChildren.size(); ++i )
        {
            if( mapxChildren[ i ]->IsSubstitution() )
            {
                ++iEnd;
            }

            mapxChildren[ i ]->TidyRecursions();
        }

        // SE - NOTE: should be equality only but paranoia.
        if( iEnd <= iBegin )
        {
            return;
        }

        // we need to insert a new node from iBegin to iEnd...
        // and replace our children suitably.
        std::vector< ASTNode* > apxNewChildren;
        std::vector< ASTNode* > apxNewGrandchildren;
        for( size_t i = iBegin; i < iEnd; ++i )
        {
            apxNewGrandchildren.push_back( new ASTNode( mapxChildren[ i ] ) );
            delete mapxChildren[ i ];
        }

        // create the new thing!!
        ASTNode* const pxNewChild = new ASTNode(
            miCursor, mxToken,
            mxProductionName, apxNewGrandchildren );

        // fix the vector up
        mapxChildren.erase(
            mapxChildren.begin() + iBegin + 1,
            mapxChildren.begin() + iEnd );
        mapxChildren[ 0 ] = pxNewChild;
    }
}

void ASTNode::DebugPrintRecursive( std::string& xWorkingString ) const
{
	xWorkingString += mxProductionName.xName;
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
    xReturnValue.resize( xA.size() );
	for( size_t i = 0; i < xA.size(); ++i )
	{
		xReturnValue[ i ] = new ASTNode( xA[ i ] );
	}

	return xReturnValue;
}

ASTNode::ASTNode( const ASTNode* const pxTemplate, ASTNode* const pxChild )
: mapxChildren( Append( DeepDuplicate( pxTemplate->mapxChildren ), pxChild ) )
, maxErrors( pxTemplate->maxErrors )
, mxProductionName( pxTemplate->mxProductionName )
, mxToken( pxTemplate->mxToken )
, miCursor( pxTemplate->miCursor )
, miErrorNumber( pxTemplate->miErrorNumber )
, mpxProduction( pxTemplate->mpxProduction )
{

}

ASTNode::ASTNode( const ASTNode* const pxTemplate )
: mapxChildren( DeepDuplicate( pxTemplate->mapxChildren ) )
, maxErrors( pxTemplate->maxErrors )
, mxProductionName( pxTemplate->mxProductionName )
, mxToken( pxTemplate->mxToken )
, miCursor( pxTemplate->miCursor )
, miErrorNumber( pxTemplate->miErrorNumber )
, mpxProduction( pxTemplate->mpxProduction )
{

}

}
