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

void to_string(std::ostream &os ,std::shared_ptr<SyntaxNode> &node, std::string indent = "", bool isLast= true);

class NumberExpressionSyntax : public SyntaxNode{
public:
    NodeKind(NumberExpression)

    Token numberToken;
    explicit NumberExpressionSyntax(Token t): numberToken(t){}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
    virtual std::string GetValue() override {return numberToken.value; }
};

class UnaryOperator : public SyntaxNode{
public:
    NodeKind(UnaryOperator)

    std::shared_ptr<SyntaxNode> param;
    Token op;

    UnaryOperator(Token &op, std::shared_ptr<SyntaxNode> &p): op(op), param(p){}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
};

class BinaryOperator : public SyntaxNode{
public:
    NodeKind(BinaryOperator)

    std::shared_ptr<SyntaxNode> param1;
    Token op;
    std::shared_ptr<SyntaxNode> param2;

    BinaryOperator(std::shared_ptr<SyntaxNode> &p1, Token &op, std::shared_ptr<SyntaxNode> &p2): param1(p1), op(op), param2(p2){}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;

    std::string GetValue() override { return op.value; }
};

class ParenthesisNode : public SyntaxNode{
public:
    NodeKind(ParenthesisNode)

    Token open;
    std::shared_ptr<SyntaxNode> inside;
    Token close;

    ParenthesisNode(Token o, std::shared_ptr<SyntaxNode> i, Token c): open(o), inside(i), close(c) {}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
};

class VariableDeclarationNode : public SyntaxNode{
public:
    NodeKind(VariableDeclarationNode)

    Token var_token;
    Token name;
    std::optional<std::shared_ptr<SyntaxNode>> init;

    VariableDeclarationNode(Token var, Token name): var_token(var), name(name) {}
    VariableDeclarationNode(Token var, Token name, Token eq, std::shared_ptr<SyntaxNode> init_node): var_token(var), name(name), init(init_node) {}

    std::vector<std::shared_ptr<SyntaxNode>> GetChildren() override;
};