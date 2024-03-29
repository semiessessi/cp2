#include "CBNFTests.h"

#include "../TestHelper.h"
#include "../../../common/cpp/ASTNode.h"
#include "../../../common/cpp/Profiling.h"
#include "../../../common/cpp/Report.h"
#include "../../../lexer/cpp/CBNFLexer.h"
#include "../../../lexer/cpp/Lexer.h"
#include "../../../parser/cpp/CBNFParser.h"
#include "../../../parser/cpp/Grammar.h"
#include "../../../parser/cpp/GrammarCompiler.h"
#include "../../../parser/cpp/Parser.h"

#include <string>

namespace CP2
{
namespace Tests
{

void DoCBNFTests()
{
	const Parser::Grammar& xCBNFGrammar = Parser::GetCBNFGrammar();
	const std::string xTest( xCBNFGrammar.GetCBNF() );

	ResetMessageReports();

	AnnounceTest( "CBNF grammar compilation" );
	StartProfiling( "CBNF grammar compilation" );

	// lex, parse, compile and compare the result...
	ASTNode* const pxAST = Parser::CBNFParse(
		Lexer::CBNFLex( "recursive-cbnf-test", xTest.c_str() ) );

	const Parser::Grammar xNewGrammar = Parser::CompileGrammar( pxAST );

	EndProfiling( "CBNF grammar compilation" );
	ExpectClean( "CBNF grammar compilation" );

	const std::string xResult( xNewGrammar.GetCBNF() );

	Expect( xResult == xTest, [ & ]()
	{
		Message( "error: Failed test: expected CBNF grammar to compile to itself" );
	} );

	ResetMessageReports();

	AnnounceTest( "generated CBNF grammar compilation" );
	StartProfiling( "generated CBNF grammar compilation" );

	ASTNode* const pxNewAST = Parser::Parse(
		Lexer::Lex(
			"recursive-cbnf-test", xTest.c_str(),
			xNewGrammar.GetLexemes(), xNewGrammar.GetComments(), xNewGrammar.GetQuotes() ), xNewGrammar );
	const Parser::Grammar xFinalGrammar = Parser::CompileGrammar( pxNewAST );
	
	EndProfiling( "generated CBNF grammar compilation" );
	ExpectClean( "generated CBNF grammar compilation" );

	const std::string xNewResult( xFinalGrammar.GetCBNF() );

	Expect( xNewResult == xTest, [ & ]()
	{
		Message( "error: Failed test: expected CBNF grammar to compile from itself" );
	} );

	Parser::Grammar xMergedGrammar;
	xMergedGrammar.Merge( xNewGrammar );

	const std::string xMerged( xMergedGrammar.GetCBNF() );

	Expect( xMerged == xTest, [ & ]()
	{
		Message( "error: Failed test: expected merged CBNF grammar to be correct" );
	} );

}

}
}
