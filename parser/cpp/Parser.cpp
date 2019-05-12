#include "Parser.h"

#include "Grammar.h"

#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/Report.h"

#include <algorithm>

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

static inline bool HandleListOrOptional(
	const bool bOptional, const bool bList, const bool bNonEmpty,
	int& iCurrentListCount, size_t& j,
	const std::vector< ParseState >& axWorkingNewStates,
	std::vector< ParseState >& axNewStates )
{
	// if its optional in anyway ...
	//const bool bNotOptional = bNonEmpty && ( iCurrentListCount == 0 );
	if( bOptional || bList )
	{
		// ... and we failed to parse anything
		if( axWorkingNewStates.size() == 0 )
		{
			// then continue with the next name
			// with the same states we just tried...
			return true;
		}

		// check if what we got back was just errors and do the same
		bool bErrorsOnly = true;
		for( const ParseState& xState : axWorkingNewStates )
		{
			bErrorsOnly = bErrorsOnly && ( ( xState.mpxAST == nullptr )
				|| ( xState.mpxAST->IsErrored() ) );
		}

		if( bErrorsOnly )
		{
			return true;
		}
	}

	if( bList ) // if its a list, and we didn't skip out ...
	{
		// ... try and parse the same thing again, add to the list
		--j;
		++iCurrentListCount;
	}
	else
	{
		iCurrentListCount = 0;
	}

	// SE - TODO: this has nothing todo with the function name.
	// cleanup the old 'new' states before forgetting them.
	for( ParseState& xState : axNewStates )
	{
		xState.Cleanup();
	}

	return false;
}

static inline bool HandleCatchAll(
	const std::vector< GrammarProduction >& axNewProductions )
{
	return ( axNewProductions.size() == 1 )
		&& ( axNewProductions[ 0 ].GetExpression().IsCatchAll() );
}

static inline bool ContinueState( size_t& k,
	const std::vector< Token >& axTokens,
	const Grammar& xGrammar,
	const std::string& xName,
	const std::vector< GrammarProduction >& axNewProductions,
	std::vector< ParseState >& axWorkingNewStates,
	std::vector< ParseState >& axNewStates )
{
	// continue where this state left off
	const int iCurrentCursor = axNewStates[ k ].miCursor;
	ASTNode* const pxBaseNode = axNewStates[ k ].mpxAST;
	// early out for error states
	if( pxBaseNode->IsErrored() )
	{
		return false;
	}

	if( iCurrentCursor >= axTokens.size() )
	{
		// report error?
		ParseState xNewState =
		{
			new ASTNode( iCurrentCursor, axTokens[ iCurrentCursor - 1 ], xName, 3001, xName ),
			iCurrentCursor - 1
		};
		axWorkingNewStates.push_back( xNewState );
		return false;
	}

	if( axNewProductions.size() == 0 )
	{
		// this is a terminal
		// does it match what we expect?
		if( xName == axTokens[ iCurrentCursor ].GetName() )
		{
			// add the terminal as the one option here.
			ParseState xNewState =
			{
				ASTNode::DuplicateAndAddChild(
					pxBaseNode,
					new ASTNode(
						iCurrentCursor, axTokens[ iCurrentCursor ], xName ) ),
				iCurrentCursor + 1
			};
			axWorkingNewStates.push_back( xNewState );
		}
		else
		{
			// report error
			ParseState xNewState =
			{
				new ASTNode( iCurrentCursor, axTokens[ iCurrentCursor ], xName, 3002, xName ),
				iCurrentCursor - 1
			};
			axWorkingNewStates.push_back( xNewState );
		}

		return false;
	}

	//
	std::vector< ParseState > axChildStates =
		ParseRecursive( axTokens, xGrammar, iCurrentCursor, axNewProductions );
	bool bAllErrors = true;
	for( ParseState& xState : axChildStates )
	{
		// only add error free states... unless they are all errors
		if( ( xState.mpxAST != nullptr )
			&& ( !xState.mpxAST->IsErrored() ) )
		{
			ParseState xNewState =
			{
				ASTNode::DuplicateAndAddChild(
					pxBaseNode,
					ASTNode::Duplicate( xState.mpxAST ) ),
				xState.miCursor
			};
			bAllErrors = false;
			axWorkingNewStates.push_back( xNewState );
		}
	}

	if( bAllErrors )
	{
		// bubble the errors upwards
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

static inline bool ParseName(
	const std::vector< Token >& axTokens,
	const std::vector< Name >& axNames,
	const Grammar& xGrammar,
	int& iCurrentListCount, size_t& j,
	std::vector< ParseState >& axWorkingNewStates,
	std::vector< ParseState >& axNewStates )
{
	axWorkingNewStates.clear();

	const std::string& xName = axNames[ j ].xName;

	// safety...
	if( xName.empty() )
	{
		return false;
	}
	const bool bNonEmpty = axNames[ j ].bNonEmpty;
	const bool bList = bNonEmpty || axNames[ j ].bList;
	const bool bOptional = axNames[ j ].bOptional;

	// do we have any productions for this thing?
	const std::vector< GrammarProduction > axNewProductions =
		xGrammar.GetProductionsForParsing( xName );

	// handle catch all ...
	if( HandleCatchAll( axNewProductions ) )
	{
		return true;
	}

	// for each state we continue from some previous states...
	for( size_t k = 0; k < axNewStates.size(); ++k )
	{
		ContinueState( k,
			axTokens, xGrammar,
			xName, axNewProductions,
			axWorkingNewStates, axNewStates );
	}

	if( HandleListOrOptional(
		bOptional, bList, bNonEmpty,
		iCurrentListCount, j,
		axWorkingNewStates, axNewStates ) )
	{
		return false;
	}

	axNewStates = axWorkingNewStates;

	return false;
}

std::vector< ParseState > ParseRecursive(
	const std::vector< Token >& axTokens, const Grammar& xGrammar,
	const int iCursor, const std::vector< GrammarProduction >& axProductions )
{
	std::vector< ParseState > axStates;
	if( axProductions.size() == 0 ) // safety, should never happen.
	{
		return axStates;
	}
	
	ASTNode xTopBaseNode( iCursor, axTokens[ iCursor ], axProductions[ 0 ].GetName() );

	// we have one list of new states we iterate on
	// and one we write the next set of new states into
	std::vector< ParseState > axNewStates;
	std::vector< ParseState > axWorkingNewStates;
	bool bAllErrors = true;
	for( size_t i = 0; i < axProductions.size(); ++i )
	{
		const std::vector< Name >& axNames =
			axProductions[ i ].GetExpression().GetFlattenedNames();

		axNewStates.clear();
		axWorkingNewStates.clear();

		// we always start with one state.
		const ParseState xInitialState =
		{
			ASTNode::Duplicate( &xTopBaseNode ),
			iCursor,
		};
		axNewStates.push_back( xInitialState );

		// parse each name in the production
		int iCurrentListCount = 0;
		for( size_t j = 0; j < axNames.size(); ++j )
		{
			if( ParseName( axTokens, axNames, xGrammar,
				iCurrentListCount, j,
				axWorkingNewStates, axNewStates ) )
			{
				break;
			}
		}

		// only return states containing no errors, unless they all errored
		bool bAnyStateSucceeded = false;
		std::copy_if( axNewStates.begin(), axNewStates.end(),
			std::back_inserter( axStates ), [ & ]( const ParseState& xState ) -> bool
			{
				const bool bCopy = ( xState.mpxAST != nullptr )
					&& !( xState.mpxAST->IsErrored() );
				bAnyStateSucceeded = bAnyStateSucceeded || bCopy;
				bAllErrors = bAllErrors && !bCopy;
				return bCopy;
			} );

		// SE - TODO: ...
		// no state succeded? return an error state containing our accumulated errors.
		if( !bAnyStateSucceeded )
		{
			axStates.insert( axStates.end(), axNewStates.begin(), axNewStates.end() );
		}
	}

	if( !bAllErrors ) // remove errors if we had successes
	{
		axStates.erase( std::remove_if( axStates.begin(), axStates.end(),
			[ & ]( const ParseState& xState ) -> bool
		{
			return ( xState.mpxAST == nullptr )
				|| ( xState.mpxAST->IsErrored() );
		} ), axStates.end() );
	}

	return axStates;
}

ASTNode* Parse( const std::vector< Token >& axTokens, const Grammar& xGrammar )
{
	const std::vector< GrammarProduction > axTopLevelProductions =
		xGrammar.GetTopLevelProductions();
	const char* const szFilename = axTokens.size()
		? ( ( axTokens[ 0 ].GetFilename() != nullptr )
			? axTokens[ 0 ].GetFilename()
			: "<unknown-file>" )
		: "<unknown-file>";
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

	// check if its just errors
	if( ( axStates[ 0 ].mpxAST != nullptr )
		&& axStates[ 0 ].mpxAST->IsErrored() )
	{
		// all we got was errors (!)
		Error( 3000, szFilename, 0, 0, "Parsing was unsuccessful!" );
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

	// check if parse was complete
	ASTNode* const pxChosenTree = axStates[ 0 ].mpxAST;
	const int iLastCursor = pxChosenTree->GetEndCursor();
	if( ( iLastCursor + 1 ) < static_cast< int >( axTokens.size() ) )
	{
		// SE - NOTE: line and column here should be from the last node...
		Error( 3003, szFilename, pxChosenTree->GetLine(), pxChosenTree->GetColumn(),
			"Incomplete parse. Unexpected token: %s", axTokens[ iLastCursor + 1 ].GetName() );
		return nullptr;
	}

	return pxChosenTree;
}

}
}
