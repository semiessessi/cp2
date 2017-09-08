#ifndef AST_VISITOR_H
#define AST_VISITOR_H

namespace CP2
{

class ASTVisitor
{

public:

	virtual void Visit( class ASTNode& xNode ) = 0;
};

}

#endif

