#include "CBNFTests.h"

#include "../TestHelper.h"
#include "../../../common/cpp/ASTNode.h"
#include "../../../common/cpp/Report.h"
#include "../../../lexer/cpp/CBNFLexer.h"
#include "../../../parser/cpp/CBNFParser.h"
#include "../../../parser/cpp/Grammar.h"
#include "../../../parser/cpp/GrammarCompiler.h"

#include <string>

namespace CP2
{
namespace Tests
{

void DoCBNFTests()
{
	const Parser::Grammar& xCBNFGrammar = Parser::GetCBNFGrammar();
	const std::string xTest( xCBNFGrammar.GetCBNF() );

	// lex, parse, compile and compare the result...
	ASTNode* const pxAST = Parser::CBNFParse(
		Lexer::CBNFLex( "recursive-cbnf-test", xTest.c_str() ) );

	const std::string xResult( Parser::CompileGrammar( pxAST ).GetCBNF() );

	Expect( xResult == xTest, [ & ]()
	{
		Message( "error: Failed test: expected CBNF grammar to compile to itself" );
	} );
}

}
}
