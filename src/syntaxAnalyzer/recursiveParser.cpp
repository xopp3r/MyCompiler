
#include "recursiveParser.hpp"
#include "../common/logger.hpp"




#define PARSER_ERROR(invalidToken, ...) \
    do { ERROR("parser error at token: '" << invalidToken.lexeme << "' ("  << \
         tokenName(invalidToken.type) << ") at " << invalidToken.position.string() << " : " << __VA_ARGS__); \
       } while(0)




MyParser::MyParser(std::function<Token(void)> NextTokenCallback)
    : IParser(NextTokenCallback), nextTokenCallback(NextTokenCallback), currentToken(nextTokenCallback()) {}



AST MyParser::buildAST(void){
    
    std::unique_ptr<Programm> root = parse();

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










// common interface for all expressions, realisation is in .tpp file
std::unique_ptr<Expression> MyParser::parseExpression(void){
    
    auto expr = parsePriority<8>();

    return expr;
}





std::vector<std::unique_ptr<Expression>> MyParser::parseFunctionCallArguments(void){
    std::vector<std::unique_ptr<Expression>> arguments;

    while (currentToken.type != TOKEN_PARENTHESES_CLOSE){
        
        // check if valid start of expression
        if (not MyParser::isTokenOneOf<TOKEN_KEYWORD_TYPE, TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_CHAR, TOKEN_INTEGER, 
            TOKEN_PARENTHESES_OPEN, TOKEN_OP_ADRESS, TOKEN_OP_DEREFERENCE, TOKEN_OP_MINUS, TOKEN_OP_NOT>(currentToken.type)) 
                break;
        
        arguments.push_back(parseExpression());

        if (currentToken.type != TOKEN_PARENTHESES_CLOSE) discardToken(TOKEN_COMMA);

    }
    
    return arguments;
}





// expression ;
std::unique_ptr<ExpressionStatement> MyParser::parseExpressionStatement(void){

    auto expr = parseExpression();

    Token tok = consumeToken(TOKEN_SEMICOLON);

    return std::make_unique<ExpressionStatement>(tok, std::move(expr));
}


// return expression ;
std::unique_ptr<ReturnStatement> MyParser::parseReturnStatement(void){

    Token tok = consumeToken(TOKEN_KEYWORD_RETURN);
    auto expr = parseExpression();
    discardToken(TOKEN_SEMICOLON);

    return std::make_unique<ReturnStatement>(tok, std::move(expr));

}




// if ( expression ) { statementSequence } (else { statementSequence })?
std::unique_ptr<IfStatement> MyParser::parseIfStatement(void){

    Token tok = consumeToken(TOKEN_KEYWORD_IF);

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

        return std::make_unique<IfStatement>(tok, std::move(condition), std::move(ifBody), std::move(elseBody));
    }

    // if there just single IF
    return std::make_unique<IfStatement>(tok, std::move(condition), std::move(ifBody), std::vector<std::unique_ptr<Statement>>(0));

}



std::unique_ptr<WhileStatement> MyParser::parseWhileStatement(void){

    Token tok = consumeToken(TOKEN_KEYWORD_WHILE);

    discardToken(TOKEN_PARENTHESES_OPEN);
    auto condition = parseExpression();
    discardToken(TOKEN_PARENTHESES_CLOSE);

    discardToken(TOKEN_BRACE_OPEN);
    auto body = parseStatementSequence();
    discardToken(TOKEN_BRACE_CLOSE);

    return std::make_unique<WhileStatement>(tok, std::move(condition), std::move(body));

}




std::unique_ptr<Statement> MyParser::parseStatement(void){

    switch (currentToken.type) {
    case TOKEN_KEYWORD_TYPE: 
        return parseVariableDeclarationStatement();

    case TOKEN_KEYWORD_IF:
        return parseIfStatement();

    case TOKEN_KEYWORD_WHILE:
        return parseWhileStatement();

    case TOKEN_KEYWORD_RETURN:
        return parseReturnStatement();

    default:
        // check if valid start of statement
        if (MyParser::isTokenOneOf<TOKEN_KEYWORD_IF, TOKEN_KEYWORD_WHILE, TOKEN_KEYWORD_TYPE, TOKEN_IDENTIFIER, 
            TOKEN_STRING, TOKEN_CHAR, TOKEN_INTEGER, TOKEN_PARENTHESES_OPEN, TOKEN_OP_ADRESS, TOKEN_OP_DEREFERENCE, 
            TOKEN_OP_MINUS, TOKEN_OP_NOT>(currentToken.type)) 
                return parseExpressionStatement();

        // else error
        PARSER_ERROR(currentToken, "Invalid statement");
        break;
    }

}




// (statement)*
std::vector<std::unique_ptr<Statement>> MyParser::parseStatementSequence(void){
    std::vector<std::unique_ptr<Statement>> statements;

    // for ( // sorry x2
    //      auto statement = parseStatement();
    //      statement != nullptr; // until end of a {block;} 
    //      statement = parseStatement()
    //     ) statements.push_back(std::move(statement));
    

    while (currentToken.type != TOKEN_BRACE_CLOSE) {

        // check if valid start of statement
        if (not MyParser::isTokenOneOf<TOKEN_KEYWORD_IF, TOKEN_KEYWORD_WHILE, TOKEN_KEYWORD_TYPE, TOKEN_IDENTIFIER, 
            TOKEN_STRING, TOKEN_CHAR, TOKEN_INTEGER, TOKEN_PARENTHESES_OPEN, TOKEN_OP_ADRESS, TOKEN_OP_DEREFERENCE, 
            TOKEN_OP_MINUS, TOKEN_OP_NOT>(currentToken.type)) 
                break;

        statements.push_back(parseStatement());
    }

    return statements;
}


// <type> <identifier> (, <type> <identifier>)*
std::vector<std::unique_ptr<VariableDeclarationStatement>> MyParser::parseFunctionDefinitionArguments(void){

    std::vector<std::unique_ptr<VariableDeclarationStatement>> args;

    auto type = consumeTokenOpt(TOKEN_KEYWORD_TYPE); 
    auto name = consumeTokenOpt(TOKEN_IDENTIFIER); 
    std::optional<Token> comma = Token(type.value().position, TOKEN_PLACEHOLDER, ""); // to bypass check at first argument
    
    while (type and name and comma) {

        args.push_back(std::make_unique<VariableDeclarationStatement>(comma.value(), type.value(), name.value()));

        comma = consumeTokenOpt(TOKEN_COMMA);
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

    Token tok = consumeToken(TOKEN_KEYWORD_FUNCTION);

    Token returnType = consumeToken(TOKEN_KEYWORD_TYPE);
    Token name = consumeToken(TOKEN_IDENTIFIER);

    discardToken(TOKEN_PARENTHESES_OPEN);
    auto arguments = parseFunctionDefinitionArguments();
    discardToken(TOKEN_PARENTHESES_CLOSE);

    discardToken(TOKEN_BRACE_OPEN);
    auto body = parseStatementSequence();
    discardToken(TOKEN_BRACE_CLOSE);

    return std::make_unique<FunctionDefinition>(tok, name, returnType, std::move(arguments), std::move(body));
}



// <type> <idenifier>;
std::unique_ptr<VariableDeclarationStatement> MyParser::parseVariableDeclarationStatement(void){

    Token type = consumeToken(TOKEN_KEYWORD_TYPE);
    Token name = consumeToken(TOKEN_IDENTIFIER);

    Token tok = (currentToken.type == TOKEN_SEMICOLON) ? consumeToken(TOKEN_SEMICOLON) : consumeToken(TOKEN_COMMA);

    return std::make_unique<VariableDeclarationStatement>(tok, type, name);
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



