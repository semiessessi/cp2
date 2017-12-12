#include "GrammarCompiler.h"

#include "Grammar.h"
#include "../../common/cpp/ASTNode.h"
#include "../../common/cpp/Escaping.h"
#include "../../common/cpp/Report.h"

namespace CP2
{
namespace Parser
{

GrammarExpression CompileExpression( ASTNode* const pxRuleExpression )
{
	if( pxRuleExpression->GetChildCount() == 0 )
	{
		// empty :/
		Warning( 4501,
			pxRuleExpression->GetFilename(),
			pxRuleExpression->GetLine(),
			pxRuleExpression->GetColumn(),
			"Empty production rule ignored" );
		return GrammarExpression( "" );

	}
	
	ASTNode* const pxSymbolNode = pxRuleExpression->GetChild( 0 );
	const std::string& xSymbolName = pxSymbolNode->GetTokenValue();
	if( pxRuleExpression->GetChildCount() == 1 )
	{
		// string or ident... or catch all
		if( pxSymbolNode->GetProductionName() == "<identifier>" )
		{
			return GrammarExpression( std::string( "<" ) + xSymbolName + ">" );
		}
		else if( pxSymbolNode->GetProductionName() == "<string>" )
		{
			return GrammarExpression( xSymbolName.substr( 1, xSymbolName.length() - 2 ) );
		}
		else if( pxSymbolNode->GetProductionName() == "..." )
		{
			return GrammarExpression( "*" );
		}

		Error( 4002, pxSymbolNode->GetFilename(), pxSymbolNode->GetLine(), pxSymbolNode->GetColumn(),
			"Unexpected %s in production rule expression", pxSymbolNode->GetProductionName().c_str() );
	}
	else if( pxRuleExpression->GetChildCount() == 2 )
	{
		// some fancy fucker.
		ASTNode* const pxFancyNode = pxRuleExpression->GetChild( 1 );
		if( pxSymbolNode->GetProductionName() != "<identifier>" )
		{
			Error( 4002, pxSymbolNode->GetFilename(), pxSymbolNode->GetLine(), pxSymbolNode->GetColumn(),
				"Unexpected %s in production rule expression", pxSymbolNode->GetProductionName().c_str() );
			return GrammarExpression( "" );
		}

		GrammarExpression xSymbolExpression( std::string( "<" ) + xSymbolName + ">" );
		if( pxFancyNode->GetProductionName() == "*" )
		{
			return !xSymbolExpression;
		}
		else if( pxFancyNode->GetProductionName() == "+" )
		{
			return +xSymbolExpression;
		}
		else if( pxFancyNode->GetProductionName() == "?" )
		{
			return ~xSymbolExpression;
		}
		else
		{
			Error( 4003, pxFancyNode->GetFilename(), pxFancyNode->GetLine(), pxFancyNode->GetColumn(),
				"Unexpected %s, expected production rule operator '*', '+' or '?'",
				pxSymbolNode->GetProductionName().c_str() );
		}
	}

	return GrammarExpression( "" );
}

Grammar CompileGrammar( ASTNode* const pxAST )
{
	if( pxAST == nullptr )
	{
		Warning( 4501, "<unknown>", 0, 0,
			"Unable to compile grammar from empty syntax tree, producing empty grammar" );
		return Grammar();
	}

	std::vector< GrammarProduction > axProductions;
	std::vector< Lexer::Comment > axComments;
	std::vector< Lexer::Quote > axQuotes;
	std::vector< std::pair< std::string, std::string > > axLexemes;
	// the top should be <grammar>
	// i feel the parser stops this being hit...
	if( pxAST->GetProductionName() != "<grammar>" )
	{
		Error( 4000, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
			"Expected <grammar> but found %s instead", pxAST->GetProductionName().c_str() );
		return Grammar( axProductions );
	}

	const int iChildCount = pxAST->GetChildCount();
	if( iChildCount == 0 )
	{
		Warning( 4500, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
			"No production rules found" );
		return Grammar( axProductions );
	}

	for( int i = 0; i < iChildCount; ++i )
	{
		ASTNode* const pxProductionAST = pxAST->GetChild( i );
		const int iProductionChildCount = pxProductionAST->GetChildCount();

		// i feel the parser stops this being hit...
		if( iProductionChildCount <= 1 )
		{
			Error( 4001, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
				"Unexpectedly short production found: %s", pxProductionAST->GetTokenValue().c_str() );
			continue;
		}

		// the first child should be the name, unless it is a special case
		// like comment, quote or lexeme
		ASTNode* const pxNameNode = pxProductionAST->GetChild( 0 );
		if( pxNameNode->GetProductionName() == "comment" )
		{
			ASTNode* const pxFirst = pxProductionAST->GetChild( 1 );
			const std::string& xStart = pxFirst->GetTokenValue();
			if( iProductionChildCount == 4 )
			{
				ASTNode* const pxLast = pxProductionAST->GetChild( 2 );
				const std::string& xEnd = pxLast->GetTokenValue();
				axComments.push_back( Lexer::Comment(
					xStart.substr( 1, xStart.length() - 2 ).c_str(),
					xEnd.substr( 1, xEnd.length() - 2 ).c_str() ) );
			}
			else
			{
				axComments.push_back( Lexer::Comment(
					xStart.substr( 1, xStart.length() - 2 ).c_str() ) );
			}

			continue;
		}
		else if( pxNameNode->GetProductionName() == "quote" )
		{
			ASTNode* const pxName = pxProductionAST->GetChild( 1 );
			const std::string& xName = pxName->GetTokenValue();
			if( iProductionChildCount == 6 )
			{
				ASTNode* const pxStart = pxProductionAST->GetChild( 2 );
				const std::string& xStart = pxStart->GetTokenValue();
				ASTNode* const pxEnd = pxProductionAST->GetChild( 3 );
				const std::string& xEnd = pxEnd->GetTokenValue();
				ASTNode* const pxEscape = pxProductionAST->GetChild( 4 );
				const std::string& xEscape = pxEscape->GetTokenValue();
				axQuotes.push_back( Lexer::Quote(
					( std::string( "<" ) + xName + ">" ).c_str(),
					CBNFQuoteUnescape( xStart.substr( 1, xStart.length() - 2 ) ).c_str(),
					CBNFQuoteUnescape( xEnd.substr( 1, xEnd.length() - 2 ) ).c_str(),
					CBNFQuoteUnescape( xEscape.substr( 1, xEscape.length() - 2 ) ).c_str() ) );
			}
			continue;
		}
		else if( pxNameNode->GetProductionName() == "lexeme" )
		{
			if( iProductionChildCount == 4 )
			{
				ASTNode* const pxName = pxProductionAST->GetChild( 1 );
				ASTNode* const pxExpression = pxProductionAST->GetChild( 2 );
				const std::string& xExpression = pxExpression->GetTokenValue();
				axLexemes.push_back(
				{
					pxName->GetTokenValue(),
					CBNFQuoteUnescape( xExpression.substr( 1, xExpression.length() - 2 ) )
				} );
			}
			continue;
		}

		// i feel the parser stops this being hit...
		if( iProductionChildCount <= 3 )
		{
			Error( 4001, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
				"Unexpectedly short production found: %s", pxProductionAST->GetTokenValue().c_str() );
			continue;
		}

		// then = (should we check?)
		// then the expression followed by the ;
		// build the first expression bit, then concatenate the rest
		ASTNode* pxWorkingChild = pxProductionAST->GetChild( 2 );
		GrammarExpression xExpression( CompileExpression( pxWorkingChild ) );
		for( int j = 3; j < ( iProductionChildCount - 1 ); ++j )
		{
			pxWorkingChild = pxProductionAST->GetChild( j );
			xExpression = xExpression + CompileExpression( pxWorkingChild );
		}

		axProductions.push_back(
			GrammarProduction(
				std::string( "<" ) + pxNameNode->GetTokenValue() + ">", xExpression ) );
	}
	
	Grammar xReturnValue( axProductions );

	for( const Lexer::Comment& xComment : axComments )
	{
		if( xComment.GetEnd() )
		{
			xReturnValue.AddBlockComment( xComment.GetStart(), xComment.GetEnd() );
		}
		else
		{
			xReturnValue.AddLineComment( xComment.GetStart() );
		}
	}

	for( const Lexer::Quote& xQuote : axQuotes )
	{
		xReturnValue.AddQuote(
			xQuote.GetName(), xQuote.GetStart(), xQuote.GetEnd(), xQuote.GetEscape() );
	}

	for( const std::pair< std::string, std::string >& xLexeme : axLexemes )
	{
		xReturnValue.AddLexeme(
			( std::string( "<" ) + xLexeme.first + ">" ).c_str(),
			xLexeme.second.c_str() );
	}

	return xReturnValue;
}

}
}
