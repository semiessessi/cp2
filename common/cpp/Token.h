// Copyright (c) 2017 Cranium Software

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace CP2
{

class Token
{
	
public:

	static const Token Null;

	Token( const char* const szPrettyName, const int iID, const bool bHasValue = false )
	: mszPrettyName( szPrettyName )
	, miID( iID )
	, mbHasValue( bHasValue )
	{

	}

	Token( const Token& xBaseToken, const char* const szFilename,
		const int iLine, const int iColumn, const std::string& xValue )
	: mxValue( xBaseToken.mbHasValue ? xValue : "" )
	, mszPrettyName( xBaseToken.mszPrettyName )
	, mszFilename( szFilename )
	, miID( xBaseToken.miID )
	, miLine( iLine )
	, miColumn( iColumn )
	, miLength( static_cast< int >( xValue.length() ) )
	, mbHasValue( xBaseToken.mbHasValue )
	{

	}

	bool IsValued() const { return mbHasValue; }
	const std::string& GetValue() const { return mxValue; }
	int GetLine() const { return miLine; }
	int GetColumn() const { return miColumn; }
	int GetLength() const { return miLength; }
	int GetID() const { return miID; }
	const char* GetName() const { return mszPrettyName; }
	const char* GetFilename() const { return mszFilename; }

private:

	std::string mxValue;
	const char* mszPrettyName;
	const char* mszFilename;	// this feels wasteful to use std::string for...
	int miID;
	int miLine;
	int miColumn;
	int miLength;
	bool mbHasValue;

};

}

#endif
