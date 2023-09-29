#include <iostream>
#include <iomanip>

#include "src/Parser/Parser.h"
#include "src/Lexer/Lexer.h"
#include "Interpreter/Visitor/Visitor.h"

int main() {

    std::string line = "var asd = 10;";
    std::cout << line << std::endl;
    //std::getline(std::cin, line);
    while (line != "exit"){
        Lexer l(line);
        auto tokens = l.parse();
        Parser p(tokens);
        auto ast = p.parseStatement();
        to_string(std::cout, ast);

        Interpreter v{};
        Context c{};
        v.Visit(ast,c);

        std::cout << "Res: " << c.result;

        std::getline(std::cin, line);
    }

    return 0;
}
