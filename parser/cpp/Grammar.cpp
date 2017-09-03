#include "Grammar.h"

#include <unordered_set>

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
		xWorkingString += ( IsOptional()
			? ( xName + "?" )
			: ( IsList() ? ( xName + "*" ) : xName ) );
	}

	if( mpxRight != nullptr )
	{
		mpxRight->GetCBNFRecursive( xWorkingString );
	}
}

std::vector< GrammarProduction > Grammar::GetTopLevelProductions() const
{
	std::vector< GrammarProduction > axTopLevelProductions;

	// for each production, add dependencies to a map...
	std::unordered_set< std::string > xLowerLevelProductions;
	const int iProductionCount = GetProductionCount();
	for( int i = 0; i < iProductionCount; ++i )
	{
		const GrammarProduction& xProduction = GetProduction( i );
		std::vector< std::string > axNames = xProduction.GetExpression().GetReferencedNames(); 
		for( size_t j = 0; j < axNames.size(); ++j )
		{
			xLowerLevelProductions.insert( axNames[ j ] );
		}
	}

	for( int i = 0; i < iProductionCount; ++i )
	{
		const GrammarProduction& xProduction = GetProduction( i );
		if( xLowerLevelProductions.find( xProduction.GetName() )
			== xLowerLevelProductions.end() )
		{
			axTopLevelProductions.push_back( xProduction );
		}
	}

	return axTopLevelProductions;
}

std::vector< GrammarProduction > Grammar::GetProductions( const std::string& xName ) const
{
	auto it = mxProductionCache.find( xName );
	if( it == mxProductionCache.end() )
	{
		std::vector< GrammarProduction > axProductions;

		std::unordered_set< std::string > xLowerLevelProductions;
		const int iProductionCount = GetProductionCount();

		for( int i = 0; i < iProductionCount; ++i )
		{
			const GrammarProduction& xProduction = GetProduction( i );
			if( xProduction.GetName() == xName )
			{
				axProductions.push_back( xProduction );
			}
		}

		mxProductionCache[ xName ] = axProductions;
	}

	return mxProductionCache[ xName ];
}

std::string Grammar::GetCBNF() const
{
	std::string xReturnValue;

	// make some effort to make this pretty...
	int iMaxProductionNameLength = 8;
	for( const GrammarProduction& xProduction : maxProductions )
	{
		if( xProduction.GetName().length() > iMaxProductionNameLength )
		{
			iMaxProductionNameLength = xProduction.GetName().length();
		}
	}

	// round up to nearest 4, and add an extra one, like tabs
	iMaxProductionNameLength += 7;
	iMaxProductionNameLength &= ~3;

	for( const GrammarProduction& xProduction : maxProductions )
	{
		std::string xName = xProduction.GetName();
		// remove angled brackets...
		if( ( xName.length() > 2 )
			&& ( xName.front() == '<' )
			&& ( xName.back() == '>' ) )
		{
			xName = xName.substr( 1, xName.length() - 2 );
		}

		xReturnValue += xName;

		const int iSpaces = iMaxProductionNameLength - static_cast< int >( xName.length() );
		for( int i = 0; i < iSpaces; ++i )
		{
			xReturnValue += " ";
		}

		xReturnValue += "=   ";

		xReturnValue += xProduction.GetExpression().GetCBNF();

		xReturnValue += " ;\r\n";
	}

	return xReturnValue;
}

void Grammar::Merge( const Grammar& xOther )
{
	maxProductions.insert( maxProductions.end(), xOther.maxProductions.begin(), xOther.maxProductions.end() );
}

}
}
