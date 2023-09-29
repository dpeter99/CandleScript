//
// Created by dpeter99 on 28/09/23.
//

#include <iostream>
#include "Visitor.h"

void ConstantNumberVisitor::VisitNode(std::shared_ptr<NumberExpressionSyntax> node, Interpreter &i, Context &c) {
    c.result = std::stoi(node->numberToken.value);
}

void BinaryOperatorVisitor::VisitNode(std::shared_ptr<BinaryOperator> node, Interpreter &i, Context &c) {
    i.Visit(node->param1, c);
    auto val_a = c.result;
    i.Visit(node->param2, c);
    auto val_b = c.result;

    if(node->op.kind == TokenKinds::ADD){
        c.result = val_a + val_b;
    } else if(node->op.kind == TokenKinds::SUBTRACT){
        c.result = val_a - val_b;
    } else if(node->op.kind == TokenKinds::MULTIPLY){
        c.result = val_a * val_b;
    } if(node->op.kind == TokenKinds::DIVIDE){
        c.result = val_a / val_b;
    }
}

void ParenthesisNodeVisitor::VisitNode(std::shared_ptr<ParenthesisNode> node, Interpreter &i, Context &c) {
    i.Visit(node->inside, c);
}
