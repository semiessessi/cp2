// Copyright (c) 2019 Cranium Software

#include "Variable.h"

#include "../../../common/cpp/Escaping.h"
#include "../../../common/cpp/Report.h"

#include <algorithm>
#include <locale>

namespace CP2
{
namespace Compiler
{

Variable::Variable( const std::string& xName )
: mxName( xName )
{

}

std::string Variable::GetValue() const
{
    CP2::Error( 6000, "???", 0, 0, "Reading from variable %s which can not be treated as a string.", mxName.c_str() );
    return "<error-string-value>";
}

Variable* Variable::GetIndexed( const int ) const
{
    CP2::Error( 6001, "???", 0, 0, "Trying to index a variable %s which can not be indexed.", mxName.c_str() );
    return nullptr;
}

int Variable::GetIndexedCount() const
{
    CP2::Error( 6002, "???", 0, 0, "Trying to count a variable %s which can not be indexed.", mxName.c_str() );
    return 0;
}

std::string Variable::GetInputName() const
{
    CP2::Error( 6003, "???", 0, 0, "Trying to get input name from variable %s which is not a production name", mxName.c_str() );
    return "";
}

std::string Variable::InputNameFromOutputName(
    const std::string& xOutputName )
{
    if( xOutputName.front() == '<' )
    {
        return xOutputName.substr( 1, xOutputName.size() - 2 );
    }

    return std::string( "\"" ) + xOutputName + "\"";
}

Variable* Variable::GetKeywords() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetIdentifiers() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetStrings() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetOperators() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetTerminators() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetSeparators() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetLexemes() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetQuotes() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetLineComments() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetBlockComments() const
{
    CP2::Error( 6004, "???", 0, 0, "Bad member access on variable %s, which is not a language", mxName.c_str() );
    return nullptr;
}

Variable* Variable::GetParse() const
{
    CP2::Error( 6005, "???", 0, 0, "Bad parse access on variable %s, which does not have a parse associated with it", mxName.c_str() );
    return nullptr;
}

void Variable::DeleteArray( const std::vector< Variable* >& xVariables )
{
    for( Variable* const pxVariable : xVariables )
    {
        delete pxVariable;
    }
}

std::string Variable::GetLowerCase() const
{
    std::string xCopy = GetValue();
    std::transform( xCopy.begin(), xCopy.end(), xCopy.begin(),
    []( char c )
    {
        return std::tolower( c, std::locale() );
    });

    return xCopy;
}

std::string Variable::StripQuotes() const
{
    const std::string xValue = GetValue();
    if( ( xValue.size() > 0 )
        && ( xValue.front() == '\"' )
        && ( xValue.back() == '\"' ) )
    {
        return xValue.substr( 1, xValue.size() - 2 );
    }

    return xValue;
}

std::string Variable::RegexEscape() const
{
    // SE - TODO: less for a specific case.
    return CP2::SlashEscape(
        CP2::SlashEscape(
            CP2::RegexEscape( GetValue() ) ) );
}

}
}
