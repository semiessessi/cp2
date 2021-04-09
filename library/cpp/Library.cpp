#include "Library.h"

#include "CommandLineHandler.h"
#include "../../lexer/cpp/Lexer.h"
#include "../../parser/cpp/Grammar.h"
#include "../../parser/cpp/GrammarExpression.h"
#include "../../parser/cpp/GrammarProduction.h"
#include "../../parser/cpp/Parser.h"

static const SwitchHandlerInitialiser kaszSwitchHandlers[1] = {};

static CommandLineHandler gxCommandLine;

static std::vector<CP2::Lexer::Rule> gaxRules;
static std::vector<CP2::Lexer::Comment> gaxComments;
static std::vector<CP2::Lexer::Quote> gaxQuotes;

static std::vector<std::vector<CP2::Token>> gaxLexerResults;
static std::vector<CP2::ASTNode*> gaxParserResults;
static std::vector<CP2::Parser::GrammarProduction> gaxProductions;
static std::vector<CP2::Parser::GrammarExpression> gaxWorkingExpressions;
static CP2::Parser::Grammar gxGrammar;

EXPORT int __cdecl clParseCommandLine(const int iArgumentCount, const char* const* const pszArguments)
{
	new ( &gxCommandLine) CommandLineHandler(
		iArgumentCount,
		pszArguments,
		0, kaszSwitchHandlers);

	if (gxCommandLine.GetReturnCode() != 0)
	{
		return gxCommandLine.GetReturnCode();
	}

    return 0;
}

EXPORT int __cdecl clHasSwitch(const char* const szSwitch)
{
	const int iCount = gxCommandLine.GetSwitchCount();
	for (int i = 0; i < iCount; ++i)
	{
		if (gxCommandLine.GetSwitch(i).substr(1) == szSwitch)
		{
			// got the switch
			return 1;
		}
	}

	return 0;
}

EXPORT const char* __cdecl clMapSwitch(const char* const szSwitch)
{
	std::string& lsxResult = gxCommandLine.MapSwitch(szSwitch);
	return lsxResult.c_str();
}

EXPORT int __cdecl clFileCount()
{
	const int iCount = gxCommandLine.GetLooseArgumentCount();
	return iCount;
}

EXPORT const char* __cdecl clGetFile(int iIndex)
{
	//const int iCount = gxCommandLine.GetLooseArgumentCount();
	return gxCommandLine.GetLooseArgument(iIndex).c_str();
}

EXPORT int __cdecl clLexerRule(const char* szName, const char* szExpression)
{
	static int lsID = 0;
	++lsID;
	//printf("Adding Rule: %s with expression %s\n", szName, szExpression);
	gaxRules.push_back(CP2::Lexer::Rule(szExpression, CP2::Token(szName, lsID, szName[0] == '<')));
	return 0;
}

EXPORT int __cdecl clLexerLineComment(const char* szLineComment)
{
	gaxComments.push_back(CP2::Lexer::Comment(szLineComment));
	return 0;
}

EXPORT int __cdecl clLexerBlockComment(const char* szStart, const char* szEnd)
{
	gaxComments.push_back(CP2::Lexer::Comment(szStart, szEnd));
	return 0;
}

EXPORT int __cdecl clLexerQuote(const char* szName, const char* szStart, const char* szEnd, const char* szEscape)
{
	gaxQuotes.push_back(CP2::Lexer::Quote(szName, szStart, szEnd, szEscape));
	return 0;
}

EXPORT int __cdecl clLex(const char* const szFilename)
{
	const std::vector< CP2::Token > xTokenList =
		CP2::Lexer::Lex(szFilename, gaxRules, gaxComments, gaxQuotes);
	gaxLexerResults.push_back(xTokenList);
	gaxParserResults.push_back(nullptr);
	return static_cast< int >( gaxLexerResults.size() ) - 1;
}

EXPORT int __cdecl clParseResetExpression()
{
	gaxWorkingExpressions.clear();

	return 0;
}

EXPORT int __cdecl clParseAppendExpression(const char* const szExpression,
	const bool bIsList, const bool bIsNonEmpty, const bool bIsOptional)
{
	CP2::Parser::GrammarExpression xExpression =
		CP2::Parser::GrammarExpression(szExpression);
	if (bIsList)
	{
		xExpression = !xExpression;
	}
	if (bIsOptional)
	{
		xExpression = ~xExpression;
	}
	if (bIsNonEmpty)
	{
		xExpression = +xExpression;
	}
	gaxWorkingExpressions.push_back(xExpression);

	return 0;
}

EXPORT int __cdecl clParseCreateProduction(const char* const szName)
{
	CP2::Parser::GrammarExpression xExpression = gaxWorkingExpressions[0];
	for (size_t i = 1; i < gaxWorkingExpressions.size(); ++i)
	{
		xExpression = xExpression + gaxWorkingExpressions[i];
	}
	gaxProductions.push_back(CP2::Parser::GrammarProduction(szName, xExpression));

	clParseResetExpression();

	return 0;
}

EXPORT int __cdecl clParseCreateGrammar(const char* const szName)
{
	gxGrammar = CP2::Parser::Grammar(gaxProductions);
	gxGrammar.SetName(szName);
	gxGrammar.SetShortName(szName);
	return 0;
}

EXPORT int __cdecl clParseLex(const int iLex)
{
	CP2::ASTNode* const pxParse = CP2::Parser::Parse(gaxLexerResults[iLex], gxGrammar);
	gaxParserResults.push_back(pxParse);

	return 0;
}