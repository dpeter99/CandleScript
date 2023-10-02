#pragma once
#include <string>
#include <algorithm>
#include <map>

class TokenKind{
    friend class TokenKinds;
public:
    std::string value;
    std::string token;

    TokenKind(std::string t, std::string token): value(t), token(token){

    }

    bool operator==(const TokenKind other) const{
        return this->value == other.value ||
                this->token == other.token;

    }
};

class TokenKinds{
public:
    static std::map<std::string, TokenKind> tokenMap;
#undef TOKEN
#define TOKEN(type,token) static TokenKind type ;
  #include "Tokens.h"
#undef TOKEN
};


struct Pos{
    std::string filename;
    size_t column = 1;
    size_t line = 1;

    Pos() = default;
    Pos(std::string file_name,size_t c, size_t l): filename(std::move(file_name)), column(c), line(l){}

    void NewLine(){
        column = 0;
        line++;
    }

    std::string to_string() const{
        return filename + ":" + std::to_string(line) + ":" + std::to_string(column);
    }
};


class Token{
public:
    TokenKind kind;
    std::string value;
    Pos position;

    Token(TokenKind kind, std::string value, Pos pos): kind(kind), value(value), position(pos){

    }

    Token(TokenKind kind, Pos pos): kind(kind), value(kind.token), position(pos){

    }

    Token(std::string value, Pos pos) : kind(TokenKinds::UNKNOWN), position(pos), value(value){
        if(TokenKinds::tokenMap.contains(value)) {
            kind = TokenKinds::tokenMap.at(value);
        }
        else{
            kind = TokenKinds::IDENTIFIER;
        }
    }
};