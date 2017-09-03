// Copyright (c) 2017 Cranium Software

#include <cstdio>

int HelpHandler( const char* const )
{
    puts( "CP2 Language Compiler" );
    puts( "" );
    puts( "cp2 <source-files>" );
    puts( "" );
    puts( "<source-files>			one or more source files" );

    return 1;
}
