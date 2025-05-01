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

    std::optional<Token> consumeTokenOpt(TokenType type); 
    Token consumeToken(TokenType type); 
    bool discardTokenOpt(TokenType type); 
    void discardToken(TokenType type); 

    std::unique_ptr<Programm> parse(void);
    std::unique_ptr<FunctionDefinition> parseFunctionDefinition(void);
    std::unique_ptr<VariableDefenitionStatement> parseFunctionDeclaration(void);
    std::unique_ptr<Statement> parseStatement(void);
    std::unique_ptr<ExpressionStatement> parseExpressionStatement(void);
    std::unique_ptr<WhileStatement> parseWhileStatement(void);
    std::unique_ptr<IfStatement> parseIfStatement(void);
    std::unique_ptr<Expression> parseExpression(void);
    std::vector<std::unique_ptr<Statement>> parseStatementSequence(void);
    std::vector<std::pair<Token, Token>> parseFunctionDefinitionArguments(void);


};




