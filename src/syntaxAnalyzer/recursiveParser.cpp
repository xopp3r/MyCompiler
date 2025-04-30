
#include "recursiveParser.hpp"
#include "../common/logger.hpp"




#define PARSER_ERROR(invalidToken, ...) \
    do { ERROR("parser error at token: " << invalidToken.lexeme << "' ("  << \
         tokenName(invalidToken.type) << ") at " << invalidToken.position.string() << "\n" << __VA_ARGS__); \
       } while(0)




MyParser::MyParser(std::function<Token(void)> NextTokenCallback)
    : IParser(NextTokenCallback), nextTokenCallback(NextTokenCallback), currentToken(nextTokenCallback()) {}



AST MyParser::buildAST(void){
    
    std::unique_ptr<Node> root = parse();

    return AST(std::move(root));
}



// optionally returns token
std::optional<Token> MyParser::consumeTokenOpt(TokenType type){
    
    if (currentToken.type != type) {
        INFO("Parser mismatch: " << tokenName(currentToken.type) << " expected, insted of '" << 
             currentToken.lexeme << "' ("  << tokenName(type) << ") at " << currentToken.position.string());
        
        return std::nullopt;
    }

    Token tok = currentToken;
    currentToken = nextTokenCallback();
    return tok;

}


// return tokens or exits with error
Token MyParser::consumeToken(TokenType type){
    
    if (currentToken.type != type) {
        PARSER_ERROR(currentToken, tokenName(type) << " expected");
    }

    Token tok = currentToken;
    currentToken = nextTokenCallback();
    return tok;

}


// skips token or exits with error
void MyParser::discardToken(TokenType type){
    
    if (currentToken.type != type) {
        PARSER_ERROR(currentToken, tokenName(type) << " expected");
    }

    currentToken = nextTokenCallback();
}




// std::unique_ptr<Expression> MyParser::parseExpression(void){
//     // TODO
// }




std::vector<std::unique_ptr<Statement>> MyParser::parseStatementSequence(void){
    std::vector<std::unique_ptr<Statement>> statements;
//     // TODO

    return statements;
}


std::vector<std::pair<Token, Token>> MyParser::parseFunctionDefenitionArguments(void){

    std::vector<std::pair<Token, Token>> args;

    auto type = consumeTokenOpt(TOKEN_KEYWORD_TYPE); 
    auto name = consumeTokenOpt(TOKEN_IDENTIFIER); 
    while (type and name) {

        args.push_back(std::pair<Token, Token>(type.value(), name.value()));

        discardToken(TOKEN_COMMA);
        type = consumeTokenOpt(TOKEN_KEYWORD_TYPE);
        name = consumeTokenOpt(TOKEN_IDENTIFIER);

    }

    if (type || name){
        ERROR("Invalid arguments at " << (type ? type.value().position.string() : name.value().position.string()));
    }

    return args;
}



std::unique_ptr<FunctionDefinition> MyParser::parseFunctionDefenition(void){

    Token returnType = consumeToken(TOKEN_KEYWORD_TYPE);
    Token name = consumeToken(TOKEN_IDENTIFIER);

    discardToken(TOKEN_PARENTHESES_OPEN);
    auto arguments = parseFunctionDefenitionArguments();
    discardToken(TOKEN_PARENTHESES_CLOSE);

    discardToken(TOKEN_BRACE_OPEN);
    auto body = parseStatementSequence();
    discardToken(TOKEN_BRACE_CLOSE);

    return std::make_unique<FunctionDefinition>(name, returnType, arguments, std::move(body));
}



std::unique_ptr<Programm> MyParser::parse(void){
    std::vector<std::unique_ptr<FunctionDefinition>> functions;

    for (auto func = parseFunctionDefenition(); func != nullptr; func = parseFunctionDefenition()){
        functions.push_back(std::move(func));
    }

    return std::make_unique<Programm>(std::move(functions));
}



