#include "Grammar.h"

#include "../../../common/cpp/Escaping.h"

#include <unordered_set>

namespace CP2
{
namespace Parser
{

std::vector< std::string > GrammarExpression::GetReferencedNames() const
{
	std::vector< std::string > axNames;

	if( !mxSymbolName.empty() )
	{
		axNames.push_back( mxSymbolName );
	}

	if( mpxLeft != nullptr )
	{
		std::vector< std::string > axOtherNames = mpxLeft->GetReferencedNames();
		axNames.insert( axNames.end(), axOtherNames.begin(), axOtherNames.end() );
	}

	if( mpxRight != nullptr )
	{
		std::vector< std::string > axOtherNames = mpxRight->GetReferencedNames();
		axNames.insert( axNames.end(), axOtherNames.begin(), axOtherNames.end() );
	}

	return axNames;
}

const std::vector< std::string >& GrammarExpression::GetFlattenedNames() const
{
	if( maxFlattenedNames.size() == 0 )
	{
		GetFlattenedNamesRecursive( maxFlattenedNames );
	}

	return maxFlattenedNames;
}

std::string GrammarExpression::GetCBNF() const
{
	std::string xReturnValue;
	GetCBNFRecursive( xReturnValue );
	return xReturnValue;
}

bool GrammarExpression::IsCatchAll() const
{
	return ( mpxLeft == nullptr ) && ( mpxRight == nullptr ) && ( mxSymbolName == "*" );
}

void GrammarExpression::GetFlattenedNamesRecursive(
	std::vector< std::string >& xWorkingVector ) const
{
	if( mpxLeft != nullptr )
	{
		mpxLeft->GetFlattenedNamesRecursive( xWorkingVector );
	}

	const std::string xName = GetName();
	if( !xName.empty() )
	{
		// SE - TODO: something less shitty...
		xWorkingVector.push_back( IsNonEmpty()
			? ( std::string( "+" ) + xName )
			: IsOptional()
				? ( std::string( "?" ) + xName )
				: ( IsList() ? ( std::string( "!" ) + xName ) : xName ) );
	}

	if( mpxRight != nullptr )
	{
		mpxRight->GetFlattenedNamesRecursive( xWorkingVector );
	}
}

void GrammarExpression::GetCBNFRecursive( std::string& xWorkingString ) const
{
	if( mpxLeft != nullptr )
	{
		mpxLeft->GetCBNFRecursive( xWorkingString );
	}

	std::string xName = GetName();
	if( !xName.empty() )
	{
		// remove angle brackets, add quotes as necessary.
		if( ( xName.length() > 2 )
			&& ( xName.front() == '<' )
			&& ( xName.back() == '>' ) )
		{
			xName = xName.substr( 1, xName.length() - 2 );
		}
		else
		{
			xName = std::string( "\"" ) + xName + std::string( "\"" );
		}

		// SE - TODO: something less shitty...
		if( !xWorkingString.empty() )
		{
			xWorkingString += " ";
		}
		xWorkingString += IsNonEmpty()   
			? ( xName + "+" ) :
			( IsOptional()
				? ( xName + "?" )
				: ( IsList() ? ( xName + "*" ) : xName ) );
	}

	if( mpxRight != nullptr )
	{
		mpxRight->GetCBNFRecursive( xWorkingString );
	}
}

std::vector< GrammarProduction > Grammar::GetTopLevelProductions() const
{
	std::vector< GrammarProduction > axTopLevelProductions;

	// for each production, add dependencies to a map...
	std::unordered_set< std::string > xLowerLevelProductions;
	const int iProductionCount = GetProductionCount();
	for( int i = 0; i < iProductionCount; ++i )
	{
		const GrammarProduction& xProduction = GetProduction( i );
		std::vector< std::string > axNames = xProduction.GetExpression().GetReferencedNames(); 
		for( size_t j = 0; j < axNames.size(); ++j )
		{
			xLowerLevelProductions.insert( axNames[ j ] );
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

void Grammar::Merge( const Grammar& xOther )
{
	maxProductions.insert( maxProductions.end(), xOther.maxProductions.begin(), xOther.maxProductions.end() );

	//for( size_t i = 0; i < xOther.GetCommentCount(); ++i )
	//{
	//	// ...
	//}
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

void Grammar::InferLexemes()
{
	// SE - NOTE: it is important that the regexes come afterwards in the list
	// so they are ignored when parsing e.g. if, the regex will match 2 long
	// but the constant string should already have matched.
	std::unordered_set< std::string > xStrings;
	for( const GrammarProduction& xProduction : maxProductions )
	{
		const std::vector< std::string >& axNames =
			xProduction.GetExpression().GetFlattenedNames();

		for( const std::string& xString : axNames )
		{
			if( xString.length() > 2 )
			{
				if( ( xString.front() != '<' )
					&& ( xString.front() != '?' )
					&& ( xString.front() != '!' )
					&& ( xString.front() != '+' ) )
				{
					xStrings.insert( xString );
				}
			}
			else if( xString.length() >= 1 )
			{
				xStrings.insert( xString );
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

}
}
