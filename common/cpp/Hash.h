#ifndef HASH_H
#define HASH_H

#include <string>

namespace CP2
{

struct Hash
{
	unsigned int auData[ 4 ];
};

Hash GetHash( const std::string_view& xInput );
std::string HashToGUID( const Hash& xHash );

}

#endif
