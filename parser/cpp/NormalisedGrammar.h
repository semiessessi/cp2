#ifndef NORMALISED_GRAMMAR_H
#define NORMALISED_GRAMMAR_H

#include "Grammar.h"

#include <unordered_map>

namespace CP2
{
namespace Parser
{

// so that left recursion can be removed...

class NormalisedGrammar
{

	struct LeftFactoring
	{
		int iOriginalRule;
		int iLeftRule;
		int iRightRule;
	};

public:

	NormalisedGrammar( const Grammar& xSourceGrammar )
	: mxOriginalGrammar( xSourceGrammar )
	, mxModifiedGrammar( Normalise( xSourceGrammar ) )
	{

	}

private:

	Grammar Normalise( const Grammar& xSourceGrammar );

	const Grammar mxOriginalGrammar;
	const Grammar mxModifiedGrammar;
	
};

}
}

#endif

