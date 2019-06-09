// Copyright (c) 2017 Cranium Software

#include <cstdio>

int HelpHandler( const char* const )
{
    puts( "CBNF Grammar Tool" );
    puts( "" );
    puts( "cbnf <source-files> -o <output-file>" );
    puts( "" );
    puts( "<source-files>			one or more source files" );
	puts( "<output-file>			an output file" );

    return 1;
}
