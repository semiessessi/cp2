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
    std::vector< std::string > axKeywords;
    std::vector< std::string > axIdentifiers;
    std::vector< std::string > axOperators;
    std::vector< std::string > axTerminators;
    std::vector< std::string > axSeparators;
    std::vector< std::string > axStrings;

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

	std::string szName = "NamelessLanguage";
	std::string szShortName;
	for( int i = 0; i < iChildCount; ++i )
	{
		ASTNode* const pxStatementAST = pxAST->GetChild( i );
		const int iStatementChildCount = pxStatementAST->GetChildCount();

		// i feel the parser stops this being hit...
		if( iStatementChildCount <= 0 )
		{
			Error( 4001, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
				"Unexpectedly short statement found: %s", pxStatementAST->GetTokenValue().c_str() );
			continue;
		}

		// the first child should be the name, unless it is a special case
		// like language, comment, quote or lexeme
        ASTNode* const pxPotentialProduction = pxStatementAST->GetChild( 0 );
        const int iProductionChildCount = pxPotentialProduction->GetChildCount();
        // SE - TODO: these things need to be in data in some map...
        if( pxPotentialProduction->GetProductionName() == "keywords" )
        {
            ASTNode* const pxTerminalList = pxStatementAST->GetChild( 1 );
            const int iTerminalCount = pxTerminalList->GetChildCount() - 2;
            for( int j = 1; j < ( iTerminalCount + 1 ); ++j )
            {
                axKeywords.push_back( pxTerminalList->GetChild( j )->GetTokenValue() );
            }
            continue;
        }
        else if( pxPotentialProduction->GetProductionName() == "identifiers" )
        {
            ASTNode* const pxTerminalList = pxStatementAST->GetChild( 1 );
            const int iTerminalCount = pxTerminalList->GetChildCount() - 2;
            for( int j = 1; j < ( iTerminalCount + 1 ); ++j )
            {
                axIdentifiers.push_back( pxTerminalList->GetChild( j )->GetTokenValue() );
            }
            continue;
        }
        else if( pxPotentialProduction->GetProductionName() == "operators" )
        {
            ASTNode* const pxTerminalList = pxStatementAST->GetChild( 1 );
            const int iTerminalCount = pxTerminalList->GetChildCount() - 2;
            for( int j = 1; j < ( iTerminalCount + 1 ); ++j )
            {
                axOperators.push_back( pxTerminalList->GetChild( j )->GetTokenValue() );
            }
            continue;
        }
        else if( pxPotentialProduction->GetProductionName() == "terminators" )
        {
            ASTNode* const pxTerminalList = pxStatementAST->GetChild( 1 );
            const int iTerminalCount = pxTerminalList->GetChildCount() - 2;
            for( int j = 1; j < ( iTerminalCount + 1 ); ++j )
            {
                axTerminators.push_back( pxTerminalList->GetChild( j )->GetTokenValue() );
            }
            continue;
        }
        else if( pxPotentialProduction->GetProductionName() == "separators" )
        {
            ASTNode* const pxTerminalList = pxStatementAST->GetChild( 1 );
            const int iTerminalCount = pxTerminalList->GetChildCount() - 2;
            for( int j = 1; j < ( iTerminalCount + 1 ); ++j )
            {
                axSeparators.push_back( pxTerminalList->GetChild( j )->GetTokenValue() );
            }
            continue;
        }
        else if( pxPotentialProduction->GetProductionName() == "strings" )
        {
            ASTNode* const pxTerminalList = pxStatementAST->GetChild( 1 );
            const int iTerminalCount = pxTerminalList->GetChildCount() - 2;
            for( int j = 1; j < ( iTerminalCount + 1 ); ++j )
            {
                axStrings.push_back( pxTerminalList->GetChild( j )->GetTokenValue() );
            }
            continue;
        }
        else if( pxPotentialProduction->GetProductionName() == "<production>" )
        {
            ASTNode* const pxNameNode = pxPotentialProduction->GetChild( 0 );
            if( pxNameNode->GetProductionName() == "language" )
            {
                if( szName == "NamelessLanguage" )
                {
                    ASTNode* const pxName = pxPotentialProduction->GetChild( 1 );
                    std::string xValue = pxName->GetTokenValue();
                    szName = xValue.substr( 1, xValue.length() - 2 );
                }

                if( iProductionChildCount > 3 )
                {
                    ASTNode* const pxName = pxPotentialProduction->GetChild( 2 );
                    std::string xValue = pxName->GetTokenValue();
                    szShortName = xValue.substr( 1, xValue.length() - 2 );
                }
                continue;
            }
            else if( pxNameNode->GetProductionName() == "comment" )
            {
                ASTNode* const pxFirst = pxPotentialProduction->GetChild( 1 );
                const std::string& xStart = pxFirst->GetTokenValue();
                if( iProductionChildCount == 4 )
                {
                    ASTNode* const pxLast = pxPotentialProduction->GetChild( 2 );
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
                ASTNode* const pxName = pxPotentialProduction->GetChild( 1 );
                const std::string& xName = pxName->GetTokenValue();
                if( iProductionChildCount == 6 )
                {
                    ASTNode* const pxStart = pxPotentialProduction->GetChild( 2 );
                    const std::string& xStart = pxStart->GetTokenValue();
                    ASTNode* const pxEnd = pxPotentialProduction->GetChild( 3 );
                    const std::string& xEnd = pxEnd->GetTokenValue();
                    ASTNode* const pxEscape = pxPotentialProduction->GetChild( 4 );
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
                    ASTNode* const pxName = pxPotentialProduction->GetChild( 1 );
                    ASTNode* const pxExpression = pxPotentialProduction->GetChild( 2 );
                    const std::string& xExpression = pxExpression->GetTokenValue();
                    axLexemes.push_back(
                        {
                            pxName->GetTokenValue(),
                            CBNFQuoteUnescape( xExpression.substr( 1, xExpression.length() - 2 ) )
                        } );
                }
                continue;
            }

            /*
            // i feel the parser stops this being hit...
            if( iStatementChildCount <= 3 )
            {
                Error( 4001, pxAST->GetFilename(), pxAST->GetLine(), pxAST->GetColumn(),
                    "Unexpectedly short production found: %s", pxStatementAST->GetTokenValue().c_str() );
                continue;
            }
            */

            // then = (should we check?)
            // then the expression followed by the ;
            // build the first expression bit, then concatenate the rest
            ASTNode* pxWorkingChild = pxPotentialProduction->GetChild( 2 );
            GrammarExpression xExpression( CompileExpression( pxWorkingChild ) );
            for( int j = 3; j < ( iStatementChildCount - 1 ); ++j )
            {
                pxWorkingChild = pxPotentialProduction->GetChild( j );
                xExpression = xExpression + CompileExpression( pxWorkingChild );
            }

            axProductions.push_back(
                GrammarProduction(
                    std::string( "<" ) + pxNameNode->GetTokenValue() + ">", xExpression ) );
        }
	}
	
	Grammar xReturnValue( axProductions );

	xReturnValue.SetName( szName.c_str() );
	if( !szShortName.empty() )
	{
		xReturnValue.SetShortName( szShortName.c_str() );
	}

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

    for( const std::string& xKeyword : axKeywords )
    {
        xReturnValue.AddKeyword( xKeyword.c_str() );
    }

    for( const std::string& xIdentifier : axIdentifiers )
    {
        xReturnValue.AddIdentifier( xIdentifier.c_str() );
    }

    for( const std::string& xOperator : axOperators )
    {
        xReturnValue.AddOperator( xOperator.c_str() );
    }

    for( const std::string& xTerminator : axTerminators )
    {
        xReturnValue.AddTerminator( xTerminator.c_str() );
    }

    for( const std::string& xSeparator : axSeparators )
    {
        xReturnValue.AddSeparator( xSeparator.c_str() );
    }

    for( const std::string& xString : axStrings )
    {
        xReturnValue.AddString( xString.c_str() );
    }

	return xReturnValue;
}

}
}
