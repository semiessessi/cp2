#include "Library.h"

#include "CommandLineHandler.h"
#include "../../lexer/cpp/Lexer.h"

static const SwitchHandlerInitialiser kaszSwitchHandlers[1] = {};

static CommandLineHandler gxCommandLine;

static std::vector<CP2::Lexer::Rule> gaxRules;
static std::vector<CP2::Lexer::Comment> gaxComments;
static std::vector<CP2::Lexer::Quote> gaxQuotes;

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

EXPORT int __cdecl clLexerRule(const char* szName, const char* szExpression)
{
	static int lsID = 0;
	++lsID;
	gaxRules.push_back(CP2::Lexer::Rule(szExpression, CP2::Token(szName, lsID)));
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
