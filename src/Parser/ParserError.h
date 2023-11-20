//
// Created by dpeter99 on 04/10/23.
//

#pragma once
#include <exception>
#include <format>

#include "Lexer/Token.h"

class ParserException : public std::exception{
public:
    Token token;

    std::string message;

    ParserException(const Token &t, const std::string &message): token(t){
        this->message = std::format("[{0}] {1}", token.position.to_string(), message);
    };

    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return message.c_str();
    }

};