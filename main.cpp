#include <iostream>
#include <iomanip>

#include "src/Parser/Parser.h"
#include "src/Lexer/Lexer.h"
#include "Interpreter/Visitor/Visitor.h"

int main() {

    std::string line =
            "var asd = 10 + 10 * ((5+4) - 4);\n"
            "10 + 0;\n"
            "asd + 5 + 2; \n"
            "asd = asd + 12;\n"
            "{\n"
            "  var a = asd + 2;\n"
            "}";

    //std::getline(std::cin, line);
    while (line != "exit"){

        std::cout << "Running: \n" << line << std::endl;

        Lexer l(line, "REPL.input");
        auto tokens = l.parse();
        Parser p(tokens, "REPL.cl");
        auto ast = p.parseProgram();

        if(ast.has_value()) {

            to_string(std::cout, ast.value());

            Interpreter v{};
            Context c{};
            v.Visit(ast.value(), c);

            std::cout << "Res: " << c.result;

        }
        std::getline(std::cin, line);
    }

    return 0;
}
