#pragma once
#include <ranges>
#include <vector>
#include <memory>
#include <iomanip>
#include "../Lexer/Token.h"

using SyntaxKind = std::string;

class SyntaxNode
{
    const SyntaxKind Unknown = "Unknown";
public:
    virtual SyntaxKind Kind() {return Unknown;};
    virtual std::vector<std::shared_ptr<SyntaxNode>> GetChildren() {return {};};
    virtual std::string GetValue() {return "";};
    //virtual bool operator==(const SyntaxNode& o) = 0;
};
#define NodeKind(kind) static inline const SyntaxKind KIND = #kind; \
SyntaxKind Kind() override { return KIND;}

void to_string(std::ostream &os, const std::shared_ptr<SyntaxNode> &node, std::string indent = "", bool isLast= true);

class Expression : public SyntaxNode{

};

class NumberExpressionSyntax : public Expression{
public:
    NodeKind(NumberExpression)

    Token numberToken;
    explicit NumberExpressionSyntax(Token t): numberToken(t){}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
    virtual std::string GetValue() override {return numberToken.value; }
};

class UnaryOperator : public Expression{
public:
    NodeKind(UnaryOperator)

    std::shared_ptr<Expression> param;
    Token op;

    UnaryOperator(Token &op, std::shared_ptr<Expression> &p): op(op), param(p){}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
};

class BinaryOperator : public Expression{
public:
    NodeKind(BinaryOperator)

    std::shared_ptr<Expression> param1;
    Token op;
    std::shared_ptr<Expression> param2;

    BinaryOperator(std::shared_ptr<Expression> &p1, Token &op, std::shared_ptr<Expression> &p2): param1(p1), op(op), param2(p2){}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;

    std::string GetValue() override { return op.value; }
};

class ParenthesisNode : public Expression{
public:
    NodeKind(ParenthesisNode)

    Token open;
    std::shared_ptr<Expression> inside;
    Token close;

    ParenthesisNode(Token o, std::shared_ptr<Expression> i, Token c): open(o), inside(i), close(c) {}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
};

class VariableExpression: public Expression{
public:
    NodeKind(VariableExpression);

    Token identifier;

    VariableExpression(Token name): identifier(name) {};

    std::string GetValue() override {
        return identifier.value;
    }
};


class Statement : public SyntaxNode{

};

class StatementList : public Statement{
public:
    NodeKind(StatementList)

    std::vector<std::shared_ptr<Statement>> statements;

    StatementList(std::vector<std::shared_ptr<Statement>> &list): statements(list) {};

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override {
        return std::vector<std::shared_ptr<SyntaxNode>>(statements.begin(), statements.end());
    }
};

class VariableDeclarationStatement : public Statement{
public:
    NodeKind(VariableDeclarationStatement)

    Token var_token;
    Token name;
    std::optional<std::shared_ptr<SyntaxNode>> init;

    VariableDeclarationStatement(Token var, Token name): var_token(var), name(name) {}
    VariableDeclarationStatement(Token var, Token name, Token eq, std::shared_ptr<SyntaxNode> init_node): var_token(var), name(name), init(init_node) {}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
};

class ExpressionStatement : public Statement{
public:
    NodeKind(ExpressionStatement)

    std::shared_ptr<Expression> expression;

    explicit ExpressionStatement(const std::shared_ptr<Expression> &exp): expression(exp) {}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
};