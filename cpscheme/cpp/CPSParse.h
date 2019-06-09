// Copyright (c) 2017 Cranium Software

#ifndef CPS_PARSER_H
#define CPS_PARSER_H

namespace CP2
{

class ASTNode;

namespace Parser
{

ASTNode* SchemeParse( const char* const szString );

}
}

#endif
