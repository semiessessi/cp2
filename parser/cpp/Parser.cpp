#include "Parser.h"

#include "Grammar.h"

#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/Report.h"

namespace CP2
{
namespace Parser
{

struct ParseState
{
	ASTNode* mpxAST;
	int miCursor;

	void Cleanup()
	{
		delete mpxAST;
		mpxAST = nullptr;
	}
};

std::vector< ParseState > ParseRecursive(
	const std::vector< Token >& axTokens, const Grammar& xGrammar,
	const int iCursor, const std::vector< GrammarProduction >& axProductions );

static inline std::vector< ParseState > CreateNewState(
	const int iCursor, ASTNode* const pxTopBaseNode )
{
	return {
	{
		ASTNode::Duplicate( pxTopBaseNode ),
		iCursor,
	} };
}

static inline bool HandleName( const Grammar& xGrammar,
	const std::vector< Token >& axTokens,
	std::vector< ParseState >& axWorkingNewStates,
	std::vector< ParseState >& axNewStates,
	const std::vector< std::string >& axNames, const size_t j )
{
	axWorkingNewStates.clear();

	const std::string& xBaseName = axNames[ j ];

	// safety...
	if( xBaseName.empty() )
	{
		return false;
	}

	// tidy up the name.
	// SE - TODO: something better than magical encodings
	const std::string xListName( &xBaseName[ 1 ] );
	const bool bNonEmpty = ( xBaseName[ 0 ] == '+' ) && ( xBaseName.length() > 1 );
	const bool bList = bNonEmpty ||
		( ( xBaseName[ 0 ] == '!' ) && ( xBaseName.length() > 1 ) );
	const bool bOptional = ( xBaseName[ 0 ] == '?' ) && ( xBaseName.length() > 1 );
	const std::string& xName = ( bList || bOptional ) ? xListName : xBaseName;

	// do we have any productions for this thing?
	const std::vector< GrammarProduction > axNewProductions =
		xGrammar.GetProductions( xName );

	// handle catch all ...
	if( axNewProductions.size() == 1 )
	{
		if( axNewProductions[ 0 ].GetExpression().IsCatchAll() )
		{
			// we are done, insert the new states, as-is
			return true;
		}
	}

	// for each state we continue from some previous states...
	for( size_t k = 0; k < axNewStates.size(); ++k )
	{
		// continue where this state left off
		const int iCurrentCursor = axNewStates[ k ].miCursor;
		ASTNode* const pxBaseNode = axNewStates[ k ].mpxAST;
		if( iCurrentCursor >= axTokens.size() )
		{
			// report error?
			return false;
		}

		if( axNewProductions.size() == 0 )
		{
			// this is a terminal
			// does it match what we expect?
			if( xName == axTokens[ iCurrentCursor ].GetName() )
			{
				// add the terminal as the one option here.
				axWorkingNewStates.push_back( {
					ASTNode::DuplicateAndAddChild(
						pxBaseNode,
						new ASTNode(
							iCurrentCursor, axTokens[ iCurrentCursor ], xName ) ),
					iCurrentCursor + 1
				} );
			}
			//else
			//{
			//	// report error?
			//}

			return false;
		}

		//
		std::vector< ParseState > axChildStates =
			ParseRecursive( axTokens, xGrammar, iCurrentCursor, axNewProductions );
		for( ParseState& xState : axChildStates )
		{
			ParseState xNewState =
			{
				ASTNode::DuplicateAndAddChild(
					pxBaseNode,
					ASTNode::Duplicate( xState.mpxAST ) ),
				xState.miCursor
			};
			axWorkingNewStates.push_back( xNewState );
		}
	}

	return false;
}


static std::vector< ParseState > ParseRecursive(
	const std::vector< Token >& axTokens, const Grammar& xGrammar,
	const int iCursor, const std::vector< GrammarProduction >& axProductions )
{
	std::vector< ParseState > axStates;
	if( axProductions.size() == 0 ) // safety, should never happen.
	{
		return axStates;
	}
	
	ASTNode* const pxTopBaseNode =
		new ASTNode( iCursor, axTokens[ iCursor ], axProductions[ 0 ].GetName() );

	for( size_t i = 0; i < axProductions.size(); ++i )
	{
		const std::vector< std::string >& axNames =
			axProductions[ i ].GetExpression().GetFlattenedNames();

		// we have one list of new states we iterate on
		// and one we write the next set of new states into
		std::vector< ParseState > axNewStates(
			CreateNewState( iCursor, pxTopBaseNode ) );
		std::vector< ParseState > axWorkingNewStates;

		// parse each name in the production
		int iCurrentListCount = 0;
		for( size_t j = 0; j < axNames.size(); ++j )
		{
			const bool bBreak = HandleName(
				xGrammar, axTokens, axWorkingNewStates, axNewStates, axNames, j );
			if( bBreak )
			{
				break;
			}
		}

		axStates.insert( axStates.end(), axNewStates.begin(), axNewStates.end() );
	}

	delete pxTopBaseNode;

	return axStates;
}

ASTNode* Parse( const std::vector< Token >& axTokens, const Grammar& xGrammar )
{
	const std::vector< GrammarProduction > axTopLevelProductions =
		xGrammar.GetTopLevelProductions();
	const char* const szFilename = axTokens.size() ? axTokens[ 0 ].GetFilename() : "<unknown-file>";
	if( axTopLevelProductions.size() == 0 )
	{
		InternalError( 3400, szFilename, 0, 0, "Grammar has no top level productions!" );
		return nullptr;
	}

	if( axTokens.size() == 0 )
	{
		Warning( 3501, szFilename, 0, 0, "No tokens to parse, ignoring input" );
		return nullptr;
	}

	std::vector< ParseState > axStates = ParseRecursive( axTokens, xGrammar, 0, axTopLevelProductions );

	if( axStates.size() == 0 )
	{
		return nullptr;
	}

	if( axStates.size() != 1 )
	{
		Warning( 3500, szFilename, 0, 0, "Parse result was ambiguous, using first valid parse" );
	}

	// delete leftovers
	for( size_t i = 1; i < axStates.size(); ++i )
	{
		axStates[ i ].Cleanup();
	}

	return axStates[ 0 ].mpxAST;
}

}
}
