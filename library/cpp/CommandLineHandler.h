// Copyright (c) 2014-2021 Cranium Software

// NOTE!! MODIFIED FROM USUAL.

#ifndef COMMAND_LINE_HANDLER_H
#define COMMAND_LINE_HANDLER_H

#include <map>
#include <string>
#include <vector>

#include "SwitchHandler.h"

class CommandLineHandler
{

public:

    CommandLineHandler() {}
    CommandLineHandler(const int iArgumentCount, const char* const* const pszArguments, const int iSwitchCount, const SwitchHandlerInitialiser* const pszSwitches)
    {
        Parse(iArgumentCount, pszArguments);
        miReturnCode = Map(iSwitchCount, pszSwitches);
    }

    int GetReturnCode() const { return miReturnCode; }
    int GetLooseArgumentCount() const { return static_cast<int>(maxUnmappedArguments.size()); }
    int GetSwitchCount() const { return static_cast<int>(maxSwitchArguments.size()); }
    //const char* const GetLooseArgument(const int i) const { return maxUnmappedArguments[i].c_str(); }

    std::string GetSwitch(const int i) const { return maxSwitchArguments[i]; }
    std::string GetLooseArgument(const int i) const { return maxUnmappedArguments[i]; }

    std::string& MapSwitch(const char* const szSwitch)
    {
        // find the switch argument
        const char kacSwitchCharacters[] = { '/', '-', '\\' };
        const int kiSwitchCharacterCount = sizeof(kacSwitchCharacters) / sizeof(kacSwitchCharacters[0]);
        const int iCount = static_cast<int>(maxParsedArguments.size());
        for (int i = 1; i < iCount; ++i) // SE - NOTE: the first one is the executable, we don't care about it...
        {
            const std::string& xCurrent = maxParsedArguments[i];
            int iSwitchOffset = 0;
            for (int j = 0; j < kiSwitchCharacterCount; ++j)
            {
                iSwitchOffset = 0;
                while (xCurrent[iSwitchOffset] == kacSwitchCharacters[j])
                {
                    ++iSwitchOffset;
                }

                if (iSwitchOffset > 0)
                {
                    break;
                }
            }

            if (iSwitchOffset != 0)
            {
                if ((std::string(szSwitch) == (&xCurrent[iSwitchOffset]))
                    || ((szSwitch[0] == xCurrent[iSwitchOffset]) && (xCurrent.length() == (iSwitchOffset + 1))))
                {
                    if ((i + 1) < maxParsedArguments.size())
                    {
                        mxMappedArguments[szSwitch] = maxParsedArguments[i + 1];
                        // remove as unmapped argument
                        (void)std::remove(maxUnmappedArguments.begin(), maxUnmappedArguments.end(), maxParsedArguments[i + 1]);
                        return mxMappedArguments[szSwitch];
                    }

                    mxMappedArguments[szSwitch] = "";
                    static std::string lsxEmpty = "";
                    return lsxEmpty;
                }
                //else if( xCurrent.substr(  ) ) // SE: something to let the parameter not be seperated by a space
            }
        }
    }

private:

    void Parse(const int iArgumentCount, const char* const* const pszArguments)
    {
        bool bInQuote = false;
        int iCurrentArgument = 0;
        for (int i = 0; i < iArgumentCount; ++i)
        {
            if (bInQuote)
            {
                std::string xCurrent = pszArguments[i];
                const int iCurrentArgumentLength = static_cast<int>(xCurrent.size());
                if (iCurrentArgumentLength > 0)
                {
                    if (xCurrent[iCurrentArgumentLength - 1] == '"')
                    {
                        bInQuote = false;
                        xCurrent.pop_back();
                    }

                    maxParsedArguments[iCurrentArgument] += xCurrent;
                }
            }
            else
            {
                //maxParsedArguments.push_back( "" );

                if (pszArguments[i][0] == 0)
                {
                    continue;
                }
                else if (pszArguments[i][0] == '"')
                {
                    bInQuote = true;

                    if (pszArguments[i][0] != 0)
                    {
                        maxParsedArguments.push_back(&(pszArguments[i][1]));
                        ++iCurrentArgument;
                    }
                }
                else
                {
                    bInQuote = false;
                    if (pszArguments[i][0] != 0)
                    {
                        maxParsedArguments.push_back(pszArguments[i]);
                        ++iCurrentArgument;
                    }
                }
            }
        }
    }

    int Map(const int iSwitchCount, const SwitchHandlerInitialiser* const pszSwitches)
    {
        const char kacSwitchCharacters[] = { '/', '-', '\\' };
        const int kiSwitchCharacterCount = sizeof(kacSwitchCharacters) / sizeof(kacSwitchCharacters[0]);
        const int iCount = static_cast<int>(maxParsedArguments.size());
        for (int i = 1; i < iCount; ++i) // SE - NOTE: the first one is the executable, we don't care about it...
        {
            const std::string& xCurrent = maxParsedArguments[i];
            int iSwitchOffset = 0;
            for (int j = 0; j < kiSwitchCharacterCount; ++j)
            {
                iSwitchOffset = 0;
                while (xCurrent[iSwitchOffset] == kacSwitchCharacters[j])
                {
                    ++iSwitchOffset;
                }

                if (iSwitchOffset > 0)
                {
                    break;
                }
            }

            if (iSwitchOffset == 0)
            {
                maxUnmappedArguments.push_back(xCurrent);
            }
            else
            {
                /*
                // is it a switch we recognise?
                for( int j = 0; j < iSwitchCount; ++j )
                {
                    const SwitchHandlerInitialiser& xSwitch = pszSwitches[ j ];
                    if( ( std::string( xSwitch.szSwitch ) == ( &xCurrent[ iSwitchOffset ] ) )
                        || ( ( xSwitch.szSwitch[ 0 ] == xCurrent[ iSwitchOffset ] ) && ( xCurrent.length() == ( iSwitchOffset + 1 ) ) ) )
                    {
                        int iReturnCode = 0;
                        if( xSwitch.bParameter )
                        {
                            mxMappedArguments[ xSwitch.szSwitch ] = maxParsedArguments[ i + 1 ];
                            iReturnCode = xSwitch.pfnHandlerFunction( maxParsedArguments[ i + 1 ].c_str( ) );
                            ++i;
                        }
                        else
                        {
                            iReturnCode = xSwitch.pfnHandlerFunction( 0 );
                        }

                        if( iReturnCode != 0 )
                        {
                            return iReturnCode;
                        }

                        break;
                    }
                    //else if( xCurrent.substr(  ) ) // SE: something to let the parameter not be seperated by a space
                }
                */

                // SE - TODO: something better here.
                maxSwitchArguments.push_back(xCurrent);
            }
        }

        return 0;
    }

    std::map< std::string, std::string > mxMappedArguments;
    std::vector< std::string > maxUnmappedArguments;
    std::vector< std::string > maxSwitchArguments;
    std::vector< std::string > maxParsedArguments;
    int miReturnCode;

};

#endif
