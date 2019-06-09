#ifndef AST_VISITOR_H
#define AST_VISITOR_H

namespace CP2
{

class ASTNode;

class ASTVisitor
{

public:

	virtual void Visit( ASTNode& xNode ) = 0;

	void DoVisit( ASTNode& xNode )
	{
		Visit( xNode );
	}

};

}

#endif

