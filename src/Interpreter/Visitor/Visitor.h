#pragma once

#include <functional>
#include <any>

#include "Parser/SyntaxNode.h"

class Interpreter;

class Context {
public:
    int result{};

    std::map<std::string, int> scope;
};

/*
class Visitor{
public:
    virtual void Visit(std::shared_ptr<SyntaxNode> node, Interpreter &i, Context &c) = 0;
};


template<typename T>
class TypedVisitor: public Visitor{
    virtual void VisitNode(std::shared_ptr<T> node, Interpreter &i, Context &c) = 0;
public:
    virtual void Visit(std::shared_ptr<SyntaxNode> node, Interpreter &i, Context &c)
    {
        this->VisitNode(std::dynamic_pointer_cast<T>(node),i, c);
    }
};
*/

class ExpressionVisitors{
public:
    static void VisitConstantNumber(std::shared_ptr<NumberExpressionSyntax> node, Interpreter &i, Context &c);

    static void VisitBinaryOperator(std::shared_ptr<BinaryOperator> node, Interpreter &i, Context &c);

    static void VisitParenthesisNode(std::shared_ptr<ParenthesisNode> node, Interpreter &i, Context &c);

    static void VisitVariableExpressionNode(std::shared_ptr<VariableExpression> node, Interpreter &i, Context &c);

    static void VisitAssignmentExpressionNode(std::shared_ptr<AssignmentExpression> node, Interpreter &i, Context &c);
};


//#################################################
//############# Statements ########################
//#################################################
class StatementVisitors{
public:
    static void VisitStatementList(std::shared_ptr<StatementList> node, Interpreter &i, Context &c);

    static void VisitVariableDeclarationStatement(std::shared_ptr<VariableDeclarationStatement> node, Interpreter &i, Context &c);

    static void VisitExpressionStatement(std::shared_ptr<ExpressionStatement> node, Interpreter &i, Context &c);
};



using Visitor = std::function<void(std::shared_ptr<SyntaxNode>, Interpreter &, Context &)>;

template<class T>
using TypedVisitor = std::function<void(std::shared_ptr<T>, Interpreter &, Context &)>;

class Interpreter {

    using VisitorRegistry = std::map<std::string, Visitor>;

    template<class NodeType>
    requires std::is_base_of_v<SyntaxNode, NodeType>
    VisitorRegistry::value_type makeVisitor(const TypedVisitor<NodeType> &visitor){
        return {NodeType::KIND, ((Visitor&)visitor)};
    }

    VisitorRegistry visitors{
            makeVisitor<StatementList>(StatementVisitors::VisitStatementList),
            makeVisitor<VariableDeclarationStatement>(StatementVisitors::VisitVariableDeclarationStatement),
            makeVisitor<ExpressionStatement>(StatementVisitors::VisitExpressionStatement),


            makeVisitor<NumberExpressionSyntax>(ExpressionVisitors::VisitConstantNumber),
            //makeVisitor<UnaryOperator>(ExpressionVisitors::Vi),
            makeVisitor<BinaryOperator>(ExpressionVisitors::VisitBinaryOperator),
            makeVisitor<ParenthesisNode>(ExpressionVisitors::VisitParenthesisNode),
            makeVisitor<VariableExpression>(ExpressionVisitors::VisitVariableExpressionNode),
            makeVisitor<AssignmentExpression>(ExpressionVisitors::VisitAssignmentExpressionNode),
    };
public:
    void Visit(std::shared_ptr<SyntaxNode> node, Context &context){

        if(visitors.contains(node->Kind())){
            visitors[node->Kind()](node, *this, context);
        }
        else{
            std::cout << std::endl << "No visitor for node:" << node->Kind() << std::endl;
        }
    }

};

