#include "LLK.h"

#include "Grammar.h"

#include <stack>
#include <unordered_set>

namespace CP2
{

namespace Parser
{

#if 0
    
bool CheckLLO( const Grammar& xGrammar, const std::vector< std::unordered_set< std::string > >& aaxFirstSets )
{
	//const std::vector< GrammarProduction > xTopLevelProductions
	//	= xGrammar.GetTopLevelProductions();

	//std::vector< GrammarProduction > axWorkingArea;
	//std::vector< GrammarProduction > axCurrentGeneration;

	return false;
}

void LLKParseTable::InitialiseFromGrammar( const Grammar& xGrammar )
{
	std::unordered_set< std::string > xTerminals( xGrammar.GetTerminals() );
	std::unordered_set< std::string > xNonTerminals( xGrammar.GetNonTerminals() );

	// build the first sets for each non-terminal
	std::vector< std::unordered_set< std::string > > aaxFirstSets;
	
	// ... for each production...
	const int iProductionCount = xGrammar.GetProductionCount();
	aaxFirstSets.resize( iProductionCount );
	for( int i = 0; i < iProductionCount; ++i )
	{
		const GrammarProduction xProduction = xGrammar.GetProduction( i );
		// note, the initialisation is not important.
		GrammarExpression xLeftmostExpression = xProduction.GetExpression().GetLeftmostChild();

		std::vector< GrammarProduction > axStack;
		axStack.push_back( xProduction );
		// so long as we are on a non-terminal, keep going.
		while( axStack.size() > 0 )
		{
			GrammarProduction xOriginalProduction = axStack.back();
			xLeftmostExpression = xOriginalProduction.GetExpression().GetLeftmostChild();
			axStack.pop_back();
			if( xNonTerminals.find( xLeftmostExpression.GetName() ) == xNonTerminals.end() )
			{
				// done.
				aaxFirstSets[ i ].emplace( xLeftmostExpression.GetName() );
			}
			else if( xLeftmostExpression.IsOptional()
				|| ( xLeftmostExpression.IsList() && !xLeftmostExpression.IsNonEmpty() ) )
			{
				// the empty symbol can do here.
				aaxFirstSets[ i ].emplace( std::string() );

				// SE - TODO: ... but what about the next symbol?!
			}

			std::vector< GrammarProduction > axProductions =
				xGrammar.GetProductions( xLeftmostExpression.GetName() );
			for( GrammarProduction xProduction : axProductions )
			{
				// avoid recursions here ...
				// ... and only look at productions that don't recurse our name.
				if( xLeftmostExpression.GetName() != xOriginalProduction.GetName() )
				{
					axStack.push_back( xProduction );
				}
			}
		}
	}

	// SE - TODO: we don't actually have the full first table yet
	// but this is enough to identify some LL(0) grammars
	if( CheckLLO( xGrammar, aaxFirstSets ) )
	{
		miK = 0;
		// SE - TODO: done? build the real table?
	}

	// SE - TODO: break point.
	puts( "" );
}

bool IsLLK( const int k,
	const Grammar& xGrammar,
	const std::vector< GrammarProduction >& axProductions )
{
	
	for( const GrammarProduction& xProduction : axProductions )
	{

	}

	return false;
}

#endif
    
}

}
