#include "GrammarCompiler.h"

#include "Grammar.h"
#include "../../common/cpp/ASTNode.h"
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
			"Unexpected %s in production rule expression", pxSymbolNode->GetProductionName() );
	}
	else if( pxRuleExpression->GetChildCount() == 2 )
	{
		// some fancy fucker.
		ASTNode* const pxFancyNode = pxRuleExpression->GetChild( 1 );
		if( pxSymbolNode->GetProductionName() != "<identifier>" )
		{
			Error( 4002, pxSymbolNode->GetFilename(), pxSymbolNode->GetLine(), pxSymbolNode->GetColumn(),
				"Unexpected %s in production rule expression", pxSymbolNode->GetProductionName() );
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
				pxSymbolNode->GetProductionName() );
		}
	}

	return GrammarExpression( "" );
}

Grammar CompileGrammar( ASTNode* const pxAST )
{
	std::vector< GrammarProduction > axProductions;

	// the top should be <grammar>
	// i feel the parser stops this being hit...
	if( pxAST->GetProductionName() != "<grammar>" )
	{
		Error( 4000, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
			"Expected <grammar> but found %s instead", pxAST->GetProductionName() );
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
		if( iProductionChildCount <= 3 )
		{
			Error( 4001, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
				"Unexpectedly short production found: %s", pxProductionAST->GetTokenValue() );
		}

		// the first child should be the name
		ASTNode* const pxNameNode = pxProductionAST->GetChild( 0 );
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
			GrammarProduction( pxNameNode->GetTokenValue(), xExpression ) );
	}
	
	return Grammar( axProductions );
}

}
}
