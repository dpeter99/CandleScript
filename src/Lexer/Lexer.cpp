#include "Lexer.h"

#include <vector>

std::vector<Token> Lexer::parse() {


    std::vector<Token> tokens;

    while(idx < buffer.size()){
        auto t  = parseToken();
        if(t.kind != TokenKinds::SPACE && t.kind != TokenKinds::NEW_LINE)
            tokens.push_back(t);

        advance();
    }
    tokens.push_back(Token(TokenKinds::EOI,pos));
    return tokens;
}

Token Lexer::parseToken() {
    char current_char = peek();

    // Handle whitespace and newline
    if(std::isspace(current_char)) {
        if(current_char == '\n'){
            this->pos.NewLine();
            return Token(TokenKinds::NEW_LINE, pos);
        }

        return Token(TokenKinds::SPACE, pos);
    }

    // Handle Single Character Tokens
    switch (current_char) {
        case ':':
            return {TokenKinds::COLON, pos};
        case ';':
            return {TokenKinds::SEMI_COLON, pos};
        case ',':
            return {TokenKinds::COMMA, pos};
        case '(':
            return {TokenKinds::PAREN_L, pos};
        case ')':
            return {TokenKinds::PAREN_R, pos};
        case '{':
            return {TokenKinds::BRACE_L, pos};
        case '}':
            return {TokenKinds::BRACE_R, pos};
        case '+':
            if(peek(1) == '+') {
                advance();
                return {TokenKinds::INCREMENT, pos};
            }
            return {TokenKinds::ADD, pos};
        case '-':
            if(peek(1) == '-') {
                advance();
                return {TokenKinds::DECREMENT, pos};
            }
            return {TokenKinds::SUBTRACT, pos};
        case '=':
            if(peek(1) == '=') {
                advance();
                return {TokenKinds::EQUAL, pos};
            }
            return {TokenKinds::ASSIGNMENT, pos};
        case '*':
            return {TokenKinds::MULTIPLY, pos};
        case '/':
            return {TokenKinds::DIVIDE, pos};
        case '\"':
            return readStringLiteral();

    }

    // Handle Keywords and Identifiers
    if(std::isalpha(current_char)) {
        std::string identifier;
        while(std::isalpha(current_char) || current_char == '_') {
            identifier += current_char;
            advance();
            current_char = peek();
        }
        advance(-1);

        return Token(identifier, pos);
    }
    if(std::isdigit(current_char)){
        return readNumberLiteral();
    }

    return Token(TokenKinds::UNKNOWN, pos);
}

Token Lexer::readStringLiteral(){
    advance(); // Move past the initial quote
    std::string literal;
    while(peek() != '"' && peek() != '\0') { // Continue until a closing quote or the end of the buffer
        literal += peek();
        advance();
    }

    if(peek() == '\0') {
        // Handle error - unclosed string literal
        // Depending on your design you might want to throw an exception,
        // return an error token, or handle it in some other way.
        return Token(TokenKinds::ERROR, "Unclosed string literal", pos);
    }

    advance(); // Move past the closing quote
    return Token(TokenKinds::STRING_LITERAL, literal, pos);
}

Token Lexer::readNumberLiteral() {

    // Handle Number Literals
    if (std::isdigit(peek()) || (peek() == '0' && std::tolower(peek(1)) == 'x')) {
        std::string numStr;

        bool isHex = (peek() == '0' && std::tolower(peek(1)) == 'x');
        bool isOct = (peek() == '0' && std::tolower(peek(1)) == 'o');
        bool hasDot = false;

        if (isHex || isOct) advance(2); // Move past '0x' or '0o'

        while (std::isdigit(peek()) || (isHex && isxdigit(peek())) || (!hasDot && peek() == '.') || peek() == 'e' || peek() == '-') {
            if (peek() == '.') hasDot = true;
            numStr += peek();
            advance();
        }
        advance(-1);

        if(isHex) numStr = "0x" + numStr; // prefix to differentiate hex
        if(isOct) numStr = "0o" + numStr; // prefix to differentiate octal

        return {TokenKinds::NUMBER_LITERAL,numStr, pos};
    }

}
