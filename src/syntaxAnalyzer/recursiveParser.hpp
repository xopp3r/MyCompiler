#pragma once

#include <optional>
#include "IParser.hpp"


class MyParser : public IParser {
    public:
    MyParser() = delete;
    ~MyParser() = default;
    MyParser(std::function<Token(void)> NextTokenCallback);

    AST buildAST() override; // return full AST

    private:
    std::function<Token(void)> nextTokenCallback; // get next token
    Token currentToken;

    void discardToken(TokenType type); 
    bool discardTokenOpt(TokenType type); 
    Token consumeToken(TokenType type); 
    std::optional<Token> consumeTokenOpt(TokenType type); 

    // parsing functions
    std::unique_ptr<Programm> parse(void);
    
    std::unique_ptr<FunctionDefinition> parseFunctionDefinition(void);
    std::vector<std::pair<Token, Token>> parseFunctionDefinitionArguments(void);
    std::vector<std::unique_ptr<Expression>> parseFunctionCallArguments(void);

    std::vector<std::unique_ptr<Statement>> parseStatementSequence(void);
    
    std::unique_ptr<Statement> parseStatement(void);
    std::unique_ptr<IfStatement> parseIfStatement(void);
    std::unique_ptr<WhileStatement> parseWhileStatement(void);
    std::unique_ptr<ReturnStatement> parseReturnStatement(void);
    std::unique_ptr<ExpressionStatement> parseExpressionStatement(void);
    std::unique_ptr<VariableDeclarationStatement> parseVariableDeclarationStatement(void);
    
    std::unique_ptr<Expression> parseExpression(void);

    #include "recursiveParser.tpp" // templated functions defenition for parsing priority
};




