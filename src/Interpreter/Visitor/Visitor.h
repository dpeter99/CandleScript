#pragma once

#include "Parser/SyntaxNode.h"

class Interpreter;

class Context {
public:
    int result{};
};

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

class ConstantNumberVisitor: public TypedVisitor<NumberExpressionSyntax>{
    void VisitNode(std::shared_ptr<NumberExpressionSyntax> node, Interpreter &i, Context &c) override;
};

class BinaryOperatorVisitor: public TypedVisitor<BinaryOperator>{
    void VisitNode(std::shared_ptr<BinaryOperator> node, Interpreter &i, Context &c) override;
};

class ParenthesisNodeVisitor: public TypedVisitor<ParenthesisNode>{
    void VisitNode(std::shared_ptr<ParenthesisNode> node, Interpreter &i, Context &c) override;
};

class Interpreter {

    std::map<std::string, std::shared_ptr<Visitor>> visitors{
            {NumberExpressionSyntax::KIND, std::make_shared<ConstantNumberVisitor>()},
            {BinaryOperator::KIND, std::make_shared<BinaryOperatorVisitor>()},
            {ParenthesisNode::KIND, std::make_shared<ParenthesisNodeVisitor>()},
    };
public:
    void Visit(std::shared_ptr<SyntaxNode> node, Context &context){

        if(visitors.contains(node->Kind())){
            visitors[node->Kind()]->Visit(node, *this, context);
        }
        else{
            std::cout << std::endl << "No visitor for node:" << node->Kind();
        }
    }

};

