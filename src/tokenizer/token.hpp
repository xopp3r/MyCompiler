#pragma once

#include <string_view>
#include "../common/position.hpp"



enum TokenType {
    TOKEN_END = 0, // end of file
    TOKEN_IDENTIFIER, // user-defined namse (variable / function name)
    TOKEN_KEYWORD, // one of reserved keywords (if / else / int ...)
    TOKEN_NUMBER, // number literal
    TOKEN_CHAR, // char literal
    TOKEN_STRING, // string literal
    TOKEN_SEMICOLON, // ;
    TOKEN_BRACE_OPEN, // {
    TOKEN_BRACE_CLOSE, // }
    TOKEN_PARENTHESES_OPEN, // (
    TOKEN_PARENTHESES_CLOSE, // )
    TOKEN_COMMA, // ,
    TOKEN_OP_PLUS, // +
    TOKEN_OP_MINUS, // -
    TOKEN_OP_MUL, // *
    TOKEN_OP_DIV, // /
    TOKEN_OP_MOD, // %
    TOKEN_OP_EQUAL, // ==
    TOKEN_OP_GREATER, // >
    TOKEN_OP_GREATER_EQ, // >=
    TOKEN_OP_LESS, // <
    TOKEN_OP_LESS_EQ, // <=
    TOKEN_OP_NOT_EQUAL, // !=
    TOKEN_OP_AND, // &&
    TOKEN_OP_OR, // ||
    TOKEN_OP_NOT, // !
    TOKEN_OP_ASSIGNMENT,  // =
    TOKEN_INVALID // ERROR
};



// Token
class Token {
    public:
    Token(Position Position, TokenType Type, std::string_view Lexeme)
        : lexeme(Lexeme), position(Position), type(Type) {}
    Token() = delete;
        
    
    std::string_view lexeme; // text of token
    Position position;
    TokenType type;
};

