#ifndef FILESYSTEM_H
#define FILESYSTEM_H

namespace CP2
{

void EnsurePath( const char* const szPath );
void WriteBinaryFile( const char* const szPath, const void* const pData, const size_t uLength );
void WriteTextFile( const char* const szPath, const char* const szText );

}

#endif
