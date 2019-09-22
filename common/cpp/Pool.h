#ifndef POOL_H
#define POOL_H

#include <vector>

namespace CP2
{

template< class T >
class Pool
{

	struct Allocation
	{
		unsigned char maucData[ sizeof( T ) ];
		bool mbFree;

		Allocation()
		: mbFree( true )
		{
		}
	};

public:

	Pool( const unsigned int uInitialSize = 33554432 )
	: maaxData()
	, muAllocated( 0 )
	, muCapacity( uInitialSize )
	, muCursor( 0 )
	, muListCursor( 0 )
	{
		maaxData.push_back( new std::vector< Allocation >() );
		maaxData.back()->resize( uInitialSize );
	}

	~Pool()
	{
		for( auto& x : maaxData )
		{
			delete x;
		}
	}

	T* Allocate()
	{
		++muAllocated;
		if( muAllocated == muCapacity )
		{
			const unsigned int uNewSize = muCapacity;
			muCapacity <<= 1;
			maaxData.push_back( new std::vector< Allocation >() );
			maaxData.back()->resize( uNewSize );
		}

		std::vector< Allocation >* pxAllocationData = maaxData[ muListCursor ];
		while( !( ( *pxAllocationData )[ muCursor ].mbFree ) )
		{
			++muCursor;
			if( muCursor == pxAllocationData->size() )
			{
				++muListCursor;
				muCursor = 0;
				if( muListCursor == maaxData.size() )
				{
					muListCursor = 0;
				}
			}
			pxAllocationData = maaxData[ muListCursor ];
		}

		( *pxAllocationData )[ muCursor ].mbFree = false;
		return reinterpret_cast< T* >( &( ( *pxAllocationData )[ muCursor ].maucData ) );
	}

	void Free( T* const pxPointer )
	{
		reinterpret_cast< Allocation* const >( pxPointer )->mbFree = true;
		--muAllocated;
	}

protected:

	std::vector< std::vector< Allocation >* > maaxData;
	unsigned int muAllocated;
	unsigned int muCapacity;
	unsigned int muCursor;
	unsigned int muListCursor;

};

template< class T >
class PoolAllocated
{

public:

	void* operator new( const size_t uSize )
	{
		return sxPool.Allocate();
	}

	void operator delete( void* const pxPointer )
	{
		sxPool.Free( reinterpret_cast< T* const >( pxPointer ) );
	}

private:

	static Pool< T > sxPool;

};

template< class T >
Pool< T > PoolAllocated< T >::sxPool;

}
#endif
