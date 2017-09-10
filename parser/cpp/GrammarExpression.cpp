#include "GrammarExpression.h"

#include "../../../common/cpp/Escaping.h"

namespace CP2
{
namespace Parser
{

std::vector< std::string > GrammarExpression::GetReferencedNames() const
{
	std::vector< std::string > axNames;

	if( !mxSymbolName.empty() )
	{
		axNames.push_back( mxSymbolName );
	}

	if( mpxLeft != nullptr )
	{
		std::vector< std::string > axOtherNames = mpxLeft->GetReferencedNames();
		axNames.insert( axNames.end(), axOtherNames.begin(), axOtherNames.end() );
	}

	if( mpxRight != nullptr )
	{
		std::vector< std::string > axOtherNames = mpxRight->GetReferencedNames();
		axNames.insert( axNames.end(), axOtherNames.begin(), axOtherNames.end() );
	}

	return axNames;
}

const std::vector< std::string >& GrammarExpression::GetFlattenedNames() const
{
	if( maxFlattenedNames.size() == 0 )
	{
		GetFlattenedNamesRecursive( maxFlattenedNames );
	}

	return maxFlattenedNames;
}

std::string GrammarExpression::GetCBNF() const
{
	std::string xReturnValue;
	GetCBNFRecursive( xReturnValue );
	return xReturnValue;
}

bool GrammarExpression::IsCatchAll() const
{
	return ( mpxLeft == nullptr ) && ( mpxRight == nullptr ) && ( mxSymbolName == "*" );
}

void GrammarExpression::GetFlattenedNamesRecursive(
	std::vector< std::string >& xWorkingVector ) const
{
	if( mpxLeft != nullptr )
	{
		mpxLeft->GetFlattenedNamesRecursive( xWorkingVector );
	}

	const std::string xName = GetName();
	if( !xName.empty() )
	{
		// SE - TODO: something less shitty...
		xWorkingVector.push_back( IsNonEmpty()
			? ( std::string( "+" ) + xName )
			: IsOptional()
			? ( std::string( "?" ) + xName )
			: ( IsList() ? ( std::string( "!" ) + xName ) : xName ) );
	}

	if( mpxRight != nullptr )
	{
		mpxRight->GetFlattenedNamesRecursive( xWorkingVector );
	}
}

void GrammarExpression::GetCBNFRecursive( std::string& xWorkingString ) const
{
	if( mpxLeft != nullptr )
	{
		mpxLeft->GetCBNFRecursive( xWorkingString );
	}

	std::string xName = GetName();
	if( !xName.empty() )
	{
		// remove angle brackets, add quotes as necessary.
		if( ( xName.length() > 2 )
			&& ( xName.front() == '<' )
			&& ( xName.back() == '>' ) )
		{
			xName = xName.substr( 1, xName.length() - 2 );
		}
		else
		{
			xName = std::string( "\"" ) + xName + std::string( "\"" );
		}

		// SE - TODO: something less shitty...
		if( !xWorkingString.empty() )
		{
			xWorkingString += " ";
		}
		xWorkingString += IsNonEmpty()   
			? ( xName + "+" ) :
			( IsOptional()
				? ( xName + "?" )
				: ( IsList() ? ( xName + "*" ) : xName ) );
	}

	if( mpxRight != nullptr )
	{
		mpxRight->GetCBNFRecursive( xWorkingString );
	}
}

}
}
