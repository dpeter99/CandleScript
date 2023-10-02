#pragma once

#include "Token.h"
#include <vector>

class Lexer {

    std::string filename;
    std::string buffer;
    size_t idx = 0;
    Pos pos;

public:
    Lexer(std::string data, std::string file_name): buffer(data), filename(file_name), pos(file_name,1,1){}

    /**
     * Parse the whole buffer
     */
    std::vector<Token> parse();

    /**
     * Parse a single token from the buffer
     * @return The parsed token
     */
    Token parseToken();

    Token readStringLiteral();

    Token readNumberLiteral();

    void advance(int n)
    {
        pos.column += n;
        idx += n;
    }
    void advance(){ advance(1); }

    char peek(int n) { return buffer[idx + n]; }
    char peek() { return peek(0); }

    bool check(int n, char c) { return peek(n) == c; }
    bool check(char c) { return check(0, c); }
};

