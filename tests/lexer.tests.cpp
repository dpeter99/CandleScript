#include "catch2/catch.hpp"
#include "../src/Lexer/Lexer.h"

TEST_CASE("Lexer handles string literals correctly", "[lexer]") {
    SECTION("Correctly formed string literal") {
        Lexer lexer("\"This is a string literal\"");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].kind == TokenKinds::STRING_LITERAL);
        REQUIRE(tokens[0].value == "This is a string literal");
    }

    SECTION("Empty string literal") {
        Lexer lexer("\"\"");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].kind == TokenKinds::STRING_LITERAL);
        REQUIRE(tokens[0].value == "");
    }

    // For unclosed string literal and error handling, you might need
    // to adapt the test case depending on how you handle errors in your lexer.
}

TEST_CASE("Lexer handles number literals correctly", "[lexer]") {
    SECTION("Integer literal") {
        Lexer lexer("12345");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].kind == TokenKinds::NUMBER_LITERAL);
        REQUIRE(tokens[0].value == "12345");
    }

    SECTION("Floating point literal") {
        Lexer lexer("123.45");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].kind == TokenKinds::NUMBER_LITERAL);
        REQUIRE(tokens[0].value == "123.45");
    }

    // Add more sections for various cases like multiple dots,
    // no digit after dot etc. depending on your error handling strategy.
}

TEST_CASE("Lexer handles single and multi-character tokens correctly", "[lexer]") {
    SECTION("Increment Token") {
        Lexer lexer("++");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].kind == TokenKinds::INCREMENT);
    }

    SECTION("Add Token") {
        Lexer lexer("+");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].kind == TokenKinds::ADD);
    }

    // Similarly, you can add sections for other tokens.
}

TEST_CASE("Lexer handles keywords and identifiers correctly", "[lexer]") {
    SECTION("Keyword - fun") {
        Lexer lexer("fun");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].kind == TokenKinds::KW_FUN);
    }

    SECTION("Identifier") {
        Lexer lexer("myIdentifier");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].kind == TokenKinds::IDENTIFIER);
        REQUIRE(tokens[0].value == "myIdentifier");
    }

    // Similarly, you can add sections for other keywords and identifiers.
}

// ... Include statements and other test cases ...

TEST_CASE("Lexer handles multiple tokens correctly", "[lexer]") {
    SECTION("Mixed tokens - function") {
        Lexer lexer("fun myFunction() { var x = 0x1a3f + 0o755 - 1.2e-3; }");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 16); // Adjust the expected size based on the actual number of tokens you expect

        REQUIRE(tokens[0].kind == TokenKinds::KW_FUN);
        REQUIRE(tokens[1].kind == TokenKinds::IDENTIFIER);
        REQUIRE(tokens[1].value == "myFunction");
        REQUIRE(tokens[2].kind == TokenKinds::PAREN_L);
        REQUIRE(tokens[3].kind == TokenKinds::PAREN_R);
        REQUIRE(tokens[4].kind == TokenKinds::BRACE_L);
        REQUIRE(tokens[5].kind == TokenKinds::KW_VAR);
        REQUIRE(tokens[6].kind == TokenKinds::IDENTIFIER);
        REQUIRE(tokens[6].value == "x");
        REQUIRE(tokens[7].kind == TokenKinds::ASSIGNMENT);
        REQUIRE(tokens[8].kind == TokenKinds::NUMBER_LITERAL);
        REQUIRE(tokens[8].value == "0x1a3f");
        REQUIRE(tokens[9].kind == TokenKinds::ADD);
        REQUIRE(tokens[10].kind == TokenKinds::NUMBER_LITERAL);
        REQUIRE(tokens[10].value == "0o755");
        REQUIRE(tokens[11].kind == TokenKinds::SUBTRACT);
        REQUIRE(tokens[12].kind == TokenKinds::NUMBER_LITERAL);
        REQUIRE(tokens[12].value == "1.2e-3");
        REQUIRE(tokens[13].kind == TokenKinds::SEMI_COLON);
        REQUIRE(tokens[14].kind == TokenKinds::BRACE_R);
    }

    SECTION("Mixed tokens - variable") {
        Lexer lexer("var asd = 10;");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 6); // Adjust the expected size based on the actual number of tokens you expect

        REQUIRE(tokens[0].kind == TokenKinds::KW_VAR);
        REQUIRE(tokens[1].kind == TokenKinds::IDENTIFIER);
        REQUIRE(tokens[1].value == "asd");
        REQUIRE(tokens[2].kind == TokenKinds::ASSIGNMENT);
        REQUIRE(tokens[3].value == "10");
        REQUIRE(tokens[3].kind == TokenKinds::NUMBER_LITERAL);
        REQUIRE(tokens[4].kind == TokenKinds::SEMI_COLON);
        REQUIRE(tokens[5].kind == TokenKinds::EOI);
    }

    SECTION("Multiple strings and numbers") {
        Lexer lexer("\"string1\" + \"string2\" == 12.34e5");
        auto tokens = lexer.parse();

        REQUIRE(tokens.size() == 6); // Adjust the expected size based on the actual number of tokens you expect

        REQUIRE(tokens[0].kind == TokenKinds::STRING_LITERAL);
        REQUIRE(tokens[0].value == "string1");
        REQUIRE(tokens[1].kind == TokenKinds::ADD);
        REQUIRE(tokens[2].kind == TokenKinds::STRING_LITERAL);
        REQUIRE(tokens[2].value == "string2");
        REQUIRE(tokens[3].kind == TokenKinds::EQUAL);
        REQUIRE(tokens[4].kind == TokenKinds::NUMBER_LITERAL);
        REQUIRE(tokens[4].value == "12.34e5");
    }

    // More test sections for different combinations and edge cases could be added here
}

// ... Other test cases ...
