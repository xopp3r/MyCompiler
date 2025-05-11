
#include <iostream>
#include <vector>
#include <format>
#include <fstream>
#include <sstream>

#include "tokenizer/MyTokenizer.hpp"
#include "syntaxAnalyzer/recursiveParser.hpp"
#include "semanticsAnalyzer/printerVisitor.hpp"


int main(int argc, char const* argv[]){

    if (argc <= 1) {
        std::cout << "No input file specified" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream sourceFile(argv[1]);

    if (not sourceFile.is_open()){
        std::cout << "Unable to open a file" << std::endl;
        return EXIT_FAILURE;
    }

    std::stringstream sourceCode;
    sourceCode << sourceFile.rdbuf();
    
    std::string sourceCodeString(sourceCode.str());
    MyTokenizer tokenizer(sourceCodeString);

    auto nextTokenCallback = [&tokenizer](){ return tokenizer.nextToken(); };
    
    MyParser parser(nextTokenCallback);
    auto ast = parser.buildAST();

    
    PrinterVisitor printer;
    printer.PrintProgramASTasHierarcy(ast.root.get());

    
    std::cout << "SUCCESS" << std::endl;
}




    // std::vector<Token> tokens;

    // for (Token t = tokenizer.nextToken(); t.type != TOKEN_END; t = tokenizer.nextToken()) {
    //     tokens.push_back(t);
    // }


    // for (const auto& token : tokens) {
    //     std::cout << std::format(
    //         "Type: <{}> Position: <{} : {} : {}> Text: <{}>", 
    //         token.typeName(), token.position.line, token.position.column, token.position.cursor, token.lexeme
    //     ) << std::endl;
    // }
