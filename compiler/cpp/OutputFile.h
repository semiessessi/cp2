// Copyright (c) 2019 Cranium Software

#ifndef OUTPUT_FILE_H
#define OUTPUT_FILE_H

#include <string>

namespace CP2
{

namespace Compiler
{

class OutputFile
{

public:

    OutputFile( 
        const std::string& xPath, const bool bBinary = false );

private:

    std::string mxPath;
    bool mbBinary;

};

}
}

#endif
