// Copyright (c) 2019 Cranium Software

#include "Output.h"

#include "../../../common/cpp/ASTNode.h"

namespace CP2
{
namespace Compiler
{

Output::Output( const std::string& xPath )
: mxPath( xPath )
{
}

Output::~Output()
{
}

void Output::Execute()
{

}

PassStatement* Output::Clone() const
{
    return new Output( mxPath );
}

void Output::GetRequiredPaths( std::vector< OutputFile >& xFiles ) const
{
    const OutputFile xOutputFile( mxPath, mbBinary );
    xFiles.push_back( xOutputFile );
}

PassStatement* Output::Create( const ASTNode* const pxAST )
{
    // SE - TODO: string expressions.
    const std::string xPath = pxAST->GetChild( 1 )->GetChild( 0 )->GetTokenValue();
    return new Output( xPath.substr( 1, xPath.size() - 2 ) );
}

}
}
