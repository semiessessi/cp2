#include "Grammar.h"

#include "../../../common/cpp/Escaping.h"

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

std::vector< GrammarProduction > Grammar::GetProductions( const std::string& xName ) const
{
	auto it = mxProductionCache.find( xName );
	if( it == mxProductionCache.end() )
	{
		std::vector< GrammarProduction > axProductions;

		std::unordered_set< std::string > xLowerLevelProductions;
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

std::string Grammar::GetCBNF() const
{
	std::string xReturnValue;

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

	RebuildTokens();

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

}
}
