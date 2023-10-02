#pragma once

#include <vector>
#include <memory>

#include "../Lexer/Token.h"
#include "SyntaxNode.h"

class Parser {

    std::vector<Token> tokens;
    size_t idx = 0;

    std::string filename;
public:
    Parser(std::vector<Token> t, const std::string &file_name): tokens(t), filename(file_name){}

    std::optional<std::shared_ptr<StatementList>> parseProgram() {

        try{
            return parseStatementList();
        } catch (const std::runtime_error& e) {
            std::printf(e.what());
        }
        return {};
    }

    std::shared_ptr<StatementList> parseStatementList();
    std::shared_ptr<Statement> parseStatement();

    std::shared_ptr<VariableDeclarationStatement> parseVariableDeclaration();
    std::shared_ptr<ExpressionStatement> parseExpressionStatement();

    std::shared_ptr<Expression> parseExpression(int precedence = 0);
    std::shared_ptr<Expression> parseParam();

    Token Consume(TokenKind kind) {
        Expect(kind);
        auto t = Peak();
        Advance();
        return t;

    }

    void Expect(TokenKind kind){
        if(!Check(kind))
            throw std::runtime_error("[" + Peak().position.to_string() + "] Expected token: " + kind.token + " but got: " + Peak().value ) ;
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

