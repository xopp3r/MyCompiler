
#include "recursiveParser.hpp"
#include "../common/logger.hpp"




#define PARSER_ERROR(invalidToken, ...) \
    do { ERROR("parser error at token: '" << invalidToken.lexeme << "' ("  << \
         tokenName(invalidToken.type) << ") at " << invalidToken.position.string() << " : " << __VA_ARGS__); \
       } while(0)




MyParser::MyParser(std::function<Token(void)> NextTokenCallback)
    : IParser(NextTokenCallback), nextTokenCallback(NextTokenCallback), currentToken(nextTokenCallback()) {}



AST MyParser::buildAST(void){
    
    std::unique_ptr<Node> root = parse();

    return AST(std::move(root));
}



// optionally consumes token and returns it if types match, else std::nullopt
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


// consumes token and returns it if types match, else exits
Token MyParser::consumeToken(TokenType type){
    
    if (currentToken.type != type) {
        PARSER_ERROR(currentToken, tokenName(type) << " expected");
    }

    Token tok = currentToken;
    currentToken = nextTokenCallback();
    return tok;

}


// skips token if types match, else exits
void MyParser::discardToken(TokenType type){
    
    if (currentToken.type != type) {
        PARSER_ERROR(currentToken, tokenName(type) << " expected");
    }

    currentToken = nextTokenCallback();
}


// skips token and returns true if types match, else false
bool MyParser::discardTokenOpt(TokenType type){
    
    if (currentToken.type != type) {
        INFO("Parser mismatch: " << tokenName(currentToken.type) << " expected, insted of '" << 
             currentToken.lexeme << "' ("  << tokenName(type) << ") at " << currentToken.position.string());
        
        return false;
    }

    currentToken = nextTokenCallback();
    return true;

}



// TODO
// common interface for all expressions
std::unique_ptr<Expression> MyParser::parseExpression(void){
    discardToken(TOKEN_IDENTIFIER);

    return nullptr;
}



// expression ;
std::unique_ptr<ExpressionStatement> MyParser::parseExpressionStatement(void){

    auto expr = parseExpression();

    discardToken(TOKEN_SEMICOLON);

    return std::make_unique<ExpressionStatement>(std::move(expr));
}



// if ( expression ) { statementSequence } (else { statementSequence })?
std::unique_ptr<IfStatement> MyParser::parseIfStatement(void){

    discardToken(TOKEN_KEYWORD_IF);

    discardToken(TOKEN_PARENTHESES_OPEN);
    auto condition = parseExpression();
    discardToken(TOKEN_PARENTHESES_CLOSE);

    discardToken(TOKEN_BRACE_OPEN);
    auto ifBody = parseStatementSequence();
    discardToken(TOKEN_BRACE_CLOSE);

    // if there is ELSE block
    if (discardTokenOpt(TOKEN_KEYWORD_ELSE)){
        discardToken(TOKEN_BRACE_OPEN);
        auto elseBody = parseStatementSequence();
        discardToken(TOKEN_BRACE_CLOSE);

        return std::make_unique<IfStatement>(std::move(condition), std::move(ifBody), std::move(elseBody));
    }

    // if there just single IF
    return std::make_unique<IfStatement>(std::move(condition), std::move(ifBody), std::vector<std::unique_ptr<Statement>>(0));

}


std::unique_ptr<WhileStatement> MyParser::parseWhileStatement(void){

    discardToken(TOKEN_KEYWORD_WHILE);

    discardToken(TOKEN_PARENTHESES_OPEN);
    auto condition = parseExpression();
    discardToken(TOKEN_PARENTHESES_CLOSE);

    discardToken(TOKEN_BRACE_OPEN);
    auto body = parseStatementSequence();
    discardToken(TOKEN_BRACE_CLOSE);

    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));

}




std::unique_ptr<Statement> MyParser::parseStatement(void){

    switch (currentToken.type) {
    // case TODO(statement;):
    // TODO return;

    case TOKEN_IDENTIFIER:
        return parseExpressionStatement();

    case TOKEN_KEYWORD_TYPE:
        return parseVariableDeclarationStatement();

    case TOKEN_KEYWORD_IF:
        return parseIfStatement();

    case TOKEN_KEYWORD_WHILE:
        return parseWhileStatement();

    case TOKEN_BRACE_CLOSE: // end of {block;}
        return nullptr;

    default:
        PARSER_ERROR(currentToken, "Invalid statement");
        break;
    }

}




// (statement)*
std::vector<std::unique_ptr<Statement>> MyParser::parseStatementSequence(void){
    std::vector<std::unique_ptr<Statement>> statements;

    for ( // sorry
         auto statement = parseStatement();
         statement != nullptr; // until end of a {block;} 
         statement = parseStatement()
        ) statements.push_back(std::move(statement));
    

    return statements;
}


// <type> <identifier> (, <type> <identifier>)*
std::vector<std::pair<Token, Token>> MyParser::parseFunctionDefinitionArguments(void){

    std::vector<std::pair<Token, Token>> args;

    auto type = consumeTokenOpt(TOKEN_KEYWORD_TYPE); 
    auto name = consumeTokenOpt(TOKEN_IDENTIFIER); 
    bool comma = type and name; // to bypass check at first argument
    while (type and name and comma) {

        args.push_back(std::pair<Token, Token>(type.value(), name.value()));

        comma = discardTokenOpt(TOKEN_COMMA);
        type = consumeTokenOpt(TOKEN_KEYWORD_TYPE);
        name = consumeTokenOpt(TOKEN_IDENTIFIER);

    }

    // inclomplete argument
    if (type or name or comma){
        ERROR("Invalid function arguments at " << currentToken.position.string());
    }

    return args;
}


// function <type> <identifier> ( args_list_defeniton ) { statementSequence }
std::unique_ptr<FunctionDefinition> MyParser::parseFunctionDefinition(void){

    discardToken(TOKEN_KEYWORD_FUNCTION);

    Token returnType = consumeToken(TOKEN_KEYWORD_TYPE);
    Token name = consumeToken(TOKEN_IDENTIFIER);

    discardToken(TOKEN_PARENTHESES_OPEN);
    auto arguments = parseFunctionDefinitionArguments();
    discardToken(TOKEN_PARENTHESES_CLOSE);

    discardToken(TOKEN_BRACE_OPEN);
    auto body = parseStatementSequence();
    discardToken(TOKEN_BRACE_CLOSE);

    return std::make_unique<FunctionDefinition>(name, returnType, std::move(arguments), std::move(body));
}



// <type> <idenifier>;
std::unique_ptr<VariableDeclarationStatement> MyParser::parseVariableDeclarationStatement(void){

    Token type = consumeToken(TOKEN_KEYWORD_TYPE);
    Token name = consumeToken(TOKEN_IDENTIFIER);

    discardToken(TOKEN_SEMICOLON);

    return std::make_unique<VariableDeclarationStatement>(type, name);
}




// (functionDefenition | globalVariableDefenition);
std::unique_ptr<Programm> MyParser::parse(void){
    std::vector<std::unique_ptr<FunctionDefinition>> functions;
    std::vector<std::unique_ptr<VariableDeclarationStatement>> globalVariables;


    while (currentToken.type != TOKEN_END){
        
        switch (currentToken.type) {
        case TOKEN_KEYWORD_FUNCTION:
            functions.push_back(parseFunctionDefinition());
            break;
        
        case TOKEN_KEYWORD_TYPE:
            globalVariables.push_back(parseVariableDeclarationStatement());
            break;

        default:
            PARSER_ERROR(currentToken, "Function or global variable defenition expected");
            break;
        }

    }
    
    return std::make_unique<Programm>(std::move(functions), std::move(globalVariables));
}



