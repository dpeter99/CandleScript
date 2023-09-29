//
// Created by dpeter99 on 25/09/23.
//

#include "Parser.h"

std::shared_ptr<SyntaxNode> Parser::parseStatement() {
    std::shared_ptr<SyntaxNode> node;
    auto n = Peak();
    if(n.kind == TokenKinds::KW_VAR){
        node = parseVariableDeclaration();
    } else{
        node = parseExp(0);
    }
    Expect(TokenKinds::SEMI_COLON);
    return node;
}

std::shared_ptr<SyntaxNode> Parser::parseVariableDeclaration(){
    auto kw = Consume(TokenKinds::KW_VAR);
    auto name = Consume(TokenKinds::IDENTIFIER);
    if(Peak().kind == TokenKinds::ASSIGNMENT){
        auto eq = Consume(TokenKinds::ASSIGNMENT);
        auto express = parseExp(0);

        return std::make_shared<VariableDeclarationNode>(kw, name, eq, express);
    }

    return std::make_shared<VariableDeclarationNode>(kw, name);
}


int prec(TokenKind op){
    if(op == TokenKinds::ADD)
        return 3;
    if(op == TokenKinds::SUBTRACT)
        return 3;
    if(op == TokenKinds::MULTIPLY)
        return 5;
    if(op == TokenKinds::DIVIDE)
        return 5;

    return 10;
}

enum class Associative{
    Left,
    Right,
};

Associative associativity(TokenKind op){
    if(op == TokenKinds::ADD)
        return Associative::Left;
    if(op == TokenKinds::SUBTRACT)
        return Associative::Left;
    if(op == TokenKinds::MULTIPLY)
        return Associative::Left;
    if(op == TokenKinds::DIVIDE)
        return Associative::Left;

    return Associative::Right;
}

bool isBinaryOperator(TokenKind op){
    if(op == TokenKinds::ADD)
        return true;
    if(op == TokenKinds::SUBTRACT)
        return true;
    if(op == TokenKinds::MULTIPLY)
        return true;
    if(op == TokenKinds::DIVIDE)
        return true;

    return false;
}

std::shared_ptr<SyntaxNode> Parser::parseExp(int precedence) {
    auto t = parseParam();
    while (isBinaryOperator(Peak().kind) && prec(Peak().kind) >= precedence){
        auto op = Peak();
        Advance();

        auto q = associativity(op.kind) == Associative::Right ?
                 prec(op.kind) :
                 1 + prec(op.kind);
        auto t1 = parseExp(q);
        t = std::make_shared<BinaryOperator>(t,op,t1);
    }
    return t;
}

std::shared_ptr<SyntaxNode> Parser::parseParam(){
    // if next is a unary operator
    if(Peak().kind == TokenKinds::SUBTRACT){
        auto op = Consume(TokenKinds::SUBTRACT);
        auto q = prec(op.kind);
        auto t = parseExp(q);
        return std::make_shared<UnaryOperator>(op, t);
    } else if (Peak().kind == TokenKinds::PAREN_L){
        auto o = Consume(TokenKinds::PAREN_L);
        auto i = parseExp(0);
        auto c = Consume(TokenKinds::PAREN_R);
        return std::make_shared<ParenthesisNode>(o,i,c);
    }
    else if (Peak().kind == TokenKinds::NUMBER_LITERAL){
        auto val = Consume(TokenKinds::NUMBER_LITERAL);
        return std::make_shared<NumberExpressionSyntax>(val);
    }
}