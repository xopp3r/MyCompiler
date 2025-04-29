

#include "recursiveParser.hpp"
#include "../common/logger.hpp"


// typedef std::unique_ptr<Node> NodeP;


static size_t& cursorRef;
static std::vector<Token>& tokensRef; 





MyParser::MyParser(std::vector<Token>& Tokens)
    : IParser(Tokens), tokens(Tokens) {
    cursorRef = cursor;
    tokensRef = tokens;
}



AST MyParser::buildAST(){
    
    std::unique_ptr<Node> root = parse();

    return AST(std::move(root));
}

