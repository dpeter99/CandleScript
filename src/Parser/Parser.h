#pragma once

#include <vector>
#include <memory>

#include "../Lexer/Token.h"
#include "SyntaxNode.h"

class Parser {

    std::vector<Token> tokens;
    size_t idx = 0;
public:
    Parser(std::vector<Token> t, std::string file_name): tokens(t) {}

    std::shared_ptr<SyntaxNode> parseStatement();

    std::shared_ptr<SyntaxNode> parseVariableDeclaration();

    std::shared_ptr<SyntaxNode> parseExp(int precedence = 0);
    std::shared_ptr<SyntaxNode> parseParam();

    Token Consume(TokenKind kind) {
        if(Check(kind)){
            auto t = Peak();
            Advance();
            return t;
        }
    }

    void Expect(TokenKind kind){
        if(!Check(kind))
            throw std::runtime_error("Expected token: " + kind.token);
    }

    bool Check(TokenKind kind) {
        return Peak().kind == kind;
    }

    Token Peak(size_t i){
        return tokens[idx+i];
    };
    Token Peak(){
        return Peak(0);
    }

    void Advance(size_t i){
        if(idx + i <= tokens.size()){
            idx += i;
        }
        else{
            throw std::out_of_range("Indexed outside of token stream (idx: " + std::to_string(idx + i) + ")");
        }
    }
    void Advance(){
        return Advance(1);
    }
};

