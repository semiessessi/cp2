#include "Grammar.h"

#include "../../common/cpp/Escaping.h"

#include <unordered_set>

namespace CP2
{
namespace Parser
{

std::vector< GrammarProduction > Grammar::GetTopLevelProductions() const
{
	std::vector< GrammarProduction > axTopLevelProductions;

	// for each production, add dependencies to a map...
	std::unordered_set< std::string > xLowerLevelProductions;
	const int iProductionCount = GetProductionCount();
	for( int i = 0; i < iProductionCount; ++i )
	{
		const GrammarProduction& xProduction = GetProduction( i );
		std::vector< Name > axNames = xProduction.GetExpression().GetReferencedNames(); 
		for( size_t j = 0; j < axNames.size(); ++j )
		{
			// e.g. in case of top level productions like a->ab
			if( axNames[ j ].xName != xProduction.GetName() )
			{
				xLowerLevelProductions.insert( axNames[ j ].xName );
			}
		}
	}

	for( int i = 0; i < iProductionCount; ++i )
	{
		const GrammarProduction& xProduction = GetProduction( i );
		if( xLowerLevelProductions.find( xProduction.GetName() )
			== xLowerLevelProductions.end() )
		{
			axTopLevelProductions.push_back( xProduction );
		}
	}

	return axTopLevelProductions;
}

std::vector< GrammarProduction > Grammar::GetOriginalProductions(
    const std::string& xName ) const
{
    auto it = mxProductionCache.find( xName );
    if( it == mxProductionCache.end() )
    {
        std::vector< GrammarProduction > axProductions;
        const int iProductionCount = GetProductionCount();

        for( int i = 0; i < iProductionCount; ++i )
        {
            const GrammarProduction& xProduction = GetProduction( i );
            if( xProduction.GetName() == xName )
            {
                axProductions.push_back( xProduction );
            }
        }


        mxProductionCache[ xName ] = axProductions;
    }

    return mxProductionCache[ xName ];
}

const std::vector< GrammarProduction >&
    Grammar::GetProductionsForParsing(
        const std::string& xName ) const
{
	auto it = mxRefactoredProductionCache.find( xName );
	if( it == mxRefactoredProductionCache.end() )
	{
        std::vector< GrammarProduction > axProductions
            = GetOriginalProductions( xName );

        // refactor the productions so that recursion avoidance is possible.
        // this kind of ruins the idea of getting what we put in.. but meh

        // because we have at this point all of the productions for this name
        // we have enough to eliminate the first level of left recursion
        // ... however to remove more will require a bit omre data.
        std::vector< GrammarProduction > axRefactoredProductions;
        const size_t iUnfactoredProductionCount
            = axProductions.size();
        for( int i = 0; i < iUnfactoredProductionCount; ++i )
        {
            const GrammarProduction& xProduction = axProductions[ i ];
            if( xProduction.IsLeftRecursive() )
            {
                for( int j = 0; j < iUnfactoredProductionCount; ++j )
                {
                    const GrammarProduction& xSubstitution = axProductions[ j ];
                    if( xSubstitution.IsLeftRecursive() == false )
                    {
                        // create a new production with the substitution
                        const GrammarProduction xSubstitutedProduction(
                            xProduction.CreateLeftmostSubstitution(
                                xProduction, xSubstitution.GetExpression() ) );
                        axRefactoredProductions.push_back( xSubstitutedProduction );
                    }
                }

                continue;
            }

            axRefactoredProductions.push_back( xProduction );
        }


        mxRefactoredProductionCache[ xName ] = axRefactoredProductions;
	}

	return mxRefactoredProductionCache[ xName ];
}

std::string Grammar::GetCBNF() const
{
	std::string xReturnValue;

	if( mszName != "NamelessLanguage" )
	{
		xReturnValue += "language \"";
		xReturnValue += mszName;
		// SE - TODO: this feels messy, its done in the name setter as well...
		std::string szTestName = mszName;
		std::transform( szTestName.begin(), szTestName.end(), szTestName.begin(), ::tolower );
		if( !mszShortName.empty() && ( mszShortName != szTestName ) )
		{
			xReturnValue += "\" ";
			xReturnValue += mszShortName;
		}
		xReturnValue += "\" ;\r\n\r\n";
	}

	for( const Lexer::Comment& xComment : maxCommentRules )
	{
		xReturnValue += "comment \"";
		xReturnValue += CBNFQuoteEscape( xComment.GetStart() );
		xReturnValue += "\"";
		if( xComment.GetEnd() )
		{
			xReturnValue += " \"";
			xReturnValue += CBNFQuoteEscape( xComment.GetEnd() );
			xReturnValue += "\"";
		}
		xReturnValue += " ;\r\n";
	}

	if( maxCommentRules.size() != 0 )
	{
		xReturnValue += "\r\n";
	}

	for( const Lexer::Quote& xQuote : maxQuoteRules )
	{
		xReturnValue += "quote ";
		const std::string xName = xQuote.GetName();
		xReturnValue += CBNFQuoteEscape( xName.substr( 1, xName.length() - 2 ) );
		xReturnValue += "";

		xReturnValue += " \"";
		xReturnValue += CBNFQuoteEscape( xQuote.GetStart() );
		xReturnValue += "\"";

		xReturnValue += " \"";
		xReturnValue += CBNFQuoteEscape( xQuote.GetEnd() );
		xReturnValue += "\"";

		xReturnValue += " \"";
		xReturnValue += CBNFQuoteEscape( xQuote.GetEscape() );
		xReturnValue += "\"";

		xReturnValue += " ;\r\n";
	}

	if( maxQuoteRules.size() != 0 )
	{
		xReturnValue += "\r\n";
	}

	bool bDoneALexeme = false;
	for( const Lexer::Rule& xLexeme : maxLexemeRules )
	{
		if( xLexeme.GetBaseToken().IsValued() )
		{
			std::string xName = xLexeme.GetBaseToken().GetName();
			xReturnValue += "lexeme ";
			xReturnValue += xName.substr( 1, xName.length() - 2 );
			xReturnValue += " \"";
			xReturnValue += CBNFQuoteEscape( xLexeme.GetExpression() );
			xReturnValue += "\" ;\r\n";
			bDoneALexeme = true;
		}
	}

	if( bDoneALexeme )
	{
		xReturnValue += "\r\n";
	}

    CreateStringForList( "keywords", maxKeywords, xReturnValue );
    CreateStringForList( "identifiers", maxIdentifiers, xReturnValue );
    CreateStringForList( "operators", maxOperators, xReturnValue );
    CreateStringForList( "terminators", maxTerminators, xReturnValue );
    CreateStringForList( "separators", maxSeparators, xReturnValue );
    CreateStringForList( "strings", maxStrings, xReturnValue );

	// make some effort to make this pretty...
	int iMaxProductionNameLength = 8;
	for( const GrammarProduction& xProduction : maxProductions )
	{
		const int iProductionNameLength =
			static_cast< int >( xProduction.GetName().length() );
		if( iProductionNameLength > iMaxProductionNameLength )
		{
			iMaxProductionNameLength = iProductionNameLength;
		}
	}

	// round up to nearest 4, and add an extra one, like tabs
	iMaxProductionNameLength += 7;
	iMaxProductionNameLength &= ~3;

	std::string xLast = maxProductions.size() ? maxProductions[ 0 ].GetName() : "";
	for( const GrammarProduction& xProduction : maxProductions )
	{
		std::string xName = xProduction.GetName();
		if( xName != xLast )
		{
			xReturnValue += "\r\n";
			xLast = xName;
		}

		int iNameLength = static_cast< int >( xName.length() );
		// remove angled brackets...
		if( ( iNameLength > 2 )
			&& ( xName.front() == '<' )
			&& ( xName.back() == '>' ) )
		{
			xName = xName.substr( 1, iNameLength - 2 );
			iNameLength -= 2;
		}

		xReturnValue += xName;

		const int iSpaces = iMaxProductionNameLength - iNameLength;
		for( int i = 0; i < iSpaces; ++i )
		{
			xReturnValue += " ";
		}

		xReturnValue += "=   ";

		xReturnValue += xProduction.GetExpression().GetCBNF();

		xReturnValue += " ;\r\n";
	}

	return xReturnValue;
}

template< typename ElementType >
static inline void MergeHelper(
	std::vector< ElementType >& axMine, const std::vector< ElementType > &axTheirs )
{
	for( const ElementType& xOtherThing : axTheirs )
	{
		bool bFound = false;
		for( const ElementType& xThing : axMine )
		{
			if( xThing == xOtherThing )
			{
				bFound = true;
				break;
			}
		}

		if( !bFound )
		{
			axMine.push_back( xOtherThing );
		}
	}
}

void Grammar::Merge( const Grammar& xOther )
{
	maxProductions.insert( maxProductions.end(), xOther.maxProductions.begin(), xOther.maxProductions.end() );

	MergeHelper( maxCommentRules, xOther.maxCommentRules );
	MergeHelper( maxLexemeRules, xOther.maxLexemeRules );
	MergeHelper( maxQuoteRules, xOther.maxQuoteRules );
    MergeHelper( maxKeywords, xOther.maxKeywords );
    MergeHelper( maxIdentifiers, xOther.maxIdentifiers );
    MergeHelper( maxOperators, xOther.maxOperators );
    MergeHelper( maxTerminators, xOther.maxTerminators );
    MergeHelper( maxSeparators, xOther.maxSeparators );
    MergeHelper( maxStrings, xOther.maxStrings );

	RebuildTokens();

	if( mszName == "NamelessLanguage" )
	{
		SetName( xOther.GetName().c_str() );
	}

}
void Grammar::AddLexeme( const char* const szPrettyName, const char* const szExpression )
{
	static const int iBaseID = 8000;
	const int iID = iBaseID + static_cast< int >( maxBaseTokens.size() );
	const char* const szPrettyNameCopy = mxTokenStrings.insert( szPrettyName ).first->c_str();
	maxBaseTokens.push_back( Token( szPrettyNameCopy, iID, true ) );
	maxLexemeRules.push_back( Lexer::Rule( szExpression, maxBaseTokens.back() ) );
}

void Grammar::AddLineComment( const char* const szStart )
{
	maxCommentRules.push_back( Lexer::Comment( szStart ) );
}

void Grammar::AddBlockComment( const char* const szStart, const char* const szEnd )
{
	maxCommentRules.push_back( Lexer::Comment( szStart, szEnd ) );
}

void Grammar::AddQuote( const char* const szName, const char* const szStart, const char* const szEnd, const char* const szEscape )
{
	maxQuoteRules.push_back( Lexer::Quote( szName, szStart, szEnd, szEscape ) );
}

std::unordered_set< std::string > Grammar::GetTerminals() const
{
	if( mxTerminals.size() == 0 )
	{
		for( Lexer::Rule xLexeme : maxLexemeRules )
		{
			mxTerminals.emplace( xLexeme.GetBaseToken().GetName() );
		}

		for( Lexer::Quote xQuotedLexeme : maxQuoteRules )
		{
			mxTerminals.emplace( xQuotedLexeme.GetName() );
		}
	}

	return mxTerminals;
}

std::unordered_set< std::string > Grammar::GetNonTerminals() const
{
	if( mxNonTerminals.size() == 0 )
	{
		for( GrammarProduction xProduction : maxProductions )
		{
			mxNonTerminals.emplace( xProduction.GetName() );
		}
	}

	return mxNonTerminals;
}

std::string Grammar::CommentText( const char* const szSource ) const
{
	// do we have any block comments?
	for( const Lexer::Comment& xComment : maxCommentRules )
	{
		if( xComment.GetEnd() != nullptr )
		{
			return ( std::string( xComment.GetStart() ) + std::string( " " )
				+ szSource ) + std::string( " " ) + xComment.GetEnd();
		}
	}

	if( maxCommentRules.size() != 0 )
	{
		// SE - TODO: line comment every line
	}

	return szSource;
}

void Grammar::InferLexemes()
{
	// SE - NOTE: it is important that the regexes come afterwards in the list
	// so they are ignored when parsing e.g. if, the regex will match 2 long
	// but the constant string should already have matched.
	std::unordered_set< std::string > xStrings;
	for( const GrammarProduction& xProduction : maxProductions )
	{
		const std::vector< Name >& axNames =
			xProduction.GetExpression().GetFlattenedNames();

		for( const Name& xName : axNames )
		{
			if( xName.xName.length() > 2 )
			{
				// SE - TODO: ...
				// argh! wtf was i thinking. this should be documented
				// with constants and comments.
				// < is a non terminal, as for the rest...
				// its ignoring lists etc... i think?!?
				if( ( xName.xName.front() != '<' )
					&& ( xName.xName.front() != '?' )
					&& ( xName.xName.front() != '!' )
					&& ( xName.xName.front() != '+' ) )
				{
					xStrings.insert( xName.xName );
				}
			}
			else if( xName.xName.length() >= 1 )
			{
				// short strings can only be a single character and not a regex
				// (SE - TODO: we hope?!)
				xStrings.insert( xName.xName );
			}
		}
	}

	int iID = 7000;
	for( const std::string& xString : xStrings )
	{
		const char* const szRuleExpression =
			mxTokenStrings.insert( RegexEscape( xString ) ).first->c_str();
		const char* const szPrettyName =
			mxTokenStrings.insert( xString ).first->c_str();
		maxBaseTokens.push_back( Token( szPrettyName, iID, false ) );
		++iID;
		maxLexemeRules.push_back( Lexer::Rule( szRuleExpression, maxBaseTokens.back() ) );
	}
}

void Grammar::RebuildTokens()
{
	maxBaseTokens.clear();
	int iID = 7000;
	for( Lexer::Rule& xRule : maxLexemeRules )
	{
		const std::string xExpression = xRule.GetExpression();
		const Token& xOriginalToken = xRule.GetBaseToken();
		const char* const szRuleExpression =
			mxTokenStrings.insert( xExpression ).first->c_str();
		const char* const szPrettyName =
			mxTokenStrings.insert( xOriginalToken.GetName() ).first->c_str();

		maxBaseTokens.push_back(
			Token( szPrettyName, iID, xOriginalToken.IsValued() ) );
		xRule = Lexer::Rule( szRuleExpression, maxBaseTokens.back() );
		++iID;
	}
}

void Grammar::EvaluateReport()
{
	// check for direct left recursions...
	for( int i = 0; i < GetProductionCount(); ++i )
	{
		const GrammarProduction& xProduction = GetProduction( i );
		if( xProduction.GetName()
			== xProduction.GetExpression().GetLeftmostChild().GetName() )
		{
			// ding ding ding!!!
			maxDirectLeftRecursions.push_back( i );
		}
	}
}

void Grammar::CreateStringForList(
    const std::string& xListName,
    const std::vector< std::string >& axList,
    std::string& xWorkingString )
{
    if( axList.size() == 0 )
    {
        return;
    }

    xWorkingString += xListName;
    xWorkingString += "\r\n";
    xWorkingString += "{\r\n";

    for( const std::string& xString : axList )
    {
        xWorkingString += "    ";
        xWorkingString += xString;
        xWorkingString += "\r\n";
    }

    xWorkingString += "}\r\n\r\n";

}

}
}
