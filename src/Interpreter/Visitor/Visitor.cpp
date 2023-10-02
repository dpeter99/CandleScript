//
// Created by dpeter99 on 28/09/23.
//

#include <iostream>
#include "Visitor.h"

void ExpressionVisitors::VisitConstantNumber(std::shared_ptr<NumberExpressionSyntax> node, Interpreter &i, Context &c) {
    c.result = std::stoi(node->numberToken.value);
}

void ExpressionVisitors::VisitBinaryOperator(std::shared_ptr<BinaryOperator> node, Interpreter &i, Context &c) {
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

void ExpressionVisitors::VisitParenthesisNode(std::shared_ptr<ParenthesisNode> node, Interpreter &i, Context &c){
    i.Visit(node->inside, c);
}

void
ExpressionVisitors::VisitVariableExpressionNode(std::shared_ptr<VariableExpression> node, Interpreter &i, Context &c) {
    c.result = c.scope[node->identifier.value];
}

void ExpressionVisitors::VisitAssignmentExpressionNode(std::shared_ptr<AssignmentExpression> node, Interpreter &i,
                                                       Context &c) {
    if(c.scope.contains(node->param1->GetValue())){
        i.Visit(node->param2,c);
        c.scope[node->param1->GetValue()] = c.result;
    }
    else {
        throw std::runtime_error("Unknown variable: " + node->param1->GetValue() );
    }
}

void StatementVisitors::VisitStatementList(std::shared_ptr<StatementList> node, Interpreter &i, Context &c) {
    for (auto &statement: node->statements) {
        i.Visit(statement, c);
    }
}

void StatementVisitors::VisitVariableDeclarationStatement(std::shared_ptr<VariableDeclarationStatement> node, Interpreter &i,
                                                    Context &c) {
    c.scope[node->name.value] = 0;
    if(node->init.has_value()){
        i.Visit(node->init.value(), c);
        c.scope[node->name.value] = c.result;
    }
}

void StatementVisitors::VisitExpressionStatement(std::shared_ptr<ExpressionStatement> node, Interpreter &i,
                                                 Context &c) {
    i.Visit(node->expression,c);
}
