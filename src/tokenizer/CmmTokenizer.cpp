
#include "CmmTokenizer.hpp"
#include "lookupTable.hpp"
#include "../common/logger.hpp"

#include <ctype.h>
#include <stdexcept>
#include <vector>



std::vector<std::string_view> keywords = {
    "break", "char", "continue",
    "else", "extern", "for", "goto", "if",
    "int", "return", "short", "void",
    "unsigned", "void", "while"
};







MyTokenizer::MyTokenizer(std::string_view Code)
    : ITokenizer(Code), code(Code) {};



bool MyTokenizer::matchPrefix(std::string_view prefix){
    
    if (cursor + prefix.length() >= code.length()) 
        return false;

    if (code.compare(cursor, prefix.length(), prefix) == 0) 
        return true;

    return false;
}



bool matchType(std::string_view str, std::vector<std::string_view>& words){

    for (const auto& word : words) {
        if (str == word) return true;
    }
    
    return false;
}



char MyTokenizer::eat(void){
    char c = code[cursor];

    if (c == '\n'){ // if end of line
        cursor++; 
        lineNumber++;
        lineBeginingCursor = cursor;
    } else {
        cursor++; // move cursor
    }

    return c;
}



std::string MyTokenizer::printTokenPos(void){
    return "Token at " + startTokenPos.string() + " - " + Position(lineNumber, cursor - lineBeginingCursor, cursor).string();
}


void MyTokenizer::updateStartTokenPos(void){
    startTokenPos.cursor = cursor;
    startTokenPos.line = lineNumber;
    startTokenPos.column = cursor - lineBeginingCursor;
}


void MyTokenizer::handleSpaces(void){

    if (cursor >= code.length()) return;

    if (matchPrefix("//")){ // skip comments

        (void) getSequence([](char c) -> bool {
            return c != '\n';
        });

    } 
    // and spaces
    (void) getSequence([](char c) -> bool {
        return isspace(c);
    });

    
}

 




Token MyTokenizer::handleIdentifier(void){
    
    std::string_view symbol = getSequence([](char c) -> bool {
        return isalnum(c) or c == '_';
    });
    

    if (matchType(symbol, keywords)){
        
        INFO("Keyword found - " << symbol);
        return Token(startTokenPos, TOKEN_KEYWORD, symbol);
        
    } else {
        
        INFO("Identifier found - " << symbol);
        return Token(startTokenPos, TOKEN_IDENTIFIER, symbol);
        
    }

}



Token MyTokenizer::handleDigitLiteral(void){

    std::string_view symbol = getSequence([](char c) -> bool { 
        return isdigit(c); 
    });

    INFO("Number literal found - " << symbol);
    return Token(startTokenPos, TOKEN_NUMBER, symbol);

}



Token MyTokenizer::handleStringLiteral(void){

    eat(); // opening quote
    
    auto matchingFunction = [](char c) -> bool {
        return c != 'c' and c != '\n';
    };

    while (matchingFunction(code[cursor]) and cursor < code.length()){
        eat();
    }

    if (cursor == code.length() or code[cursor] != '"') ERROR("Missing closing quote at " << printTokenPos());

    eat(); // closing quote


    std::string_view symbol = code.substr(startTokenPos.cursor, cursor - startTokenPos.cursor);

    INFO("String literal found  - " << symbol);
    return Token(startTokenPos, TOKEN_STRING, symbol);
}



Token MyTokenizer::handleCharLiteral(void){

    eat(); // opening quote

    std::string_view symbol = getSequence([](char c) -> bool {
        return c != 39; // ascii(39) the single quote ( ' )
    });

    if (cursor == code.length() or code[cursor] != 39) ERROR("Missing closing quote at " << printTokenPos());

    eat(); // closing quote

    if (symbol.length() != 1) ERROR("Invalid char literal at " << printTokenPos());

    INFO("Char literal found  - " << symbol);
    return Token(startTokenPos, TOKEN_CHAR, symbol);
}



Token MyTokenizer::handleOperator(void){

    // TOKEN_OP_PLUS, // +
    // TOKEN_OP_MINUS, // -
    // TOKEN_OP_MUL, // *
    // TOKEN_OP_DIV, // /
    // TOKEN_OP_MOD, // %
    // TOKEN_OP_GREATER, // >
    // TOKEN_OP_GREATER_EQ, // >=
    // TOKEN_OP_LESS, // <
    // TOKEN_OP_LESS_EQ, // <=
    // TOKEN_OP_NOT, // !
    // TOKEN_OP_NOT_EQUAL, // !=
    // TOKEN_OP_AND, // &&
    // TOKEN_OP_OR, // ||
    // TOKEN_OP_ASSIGNMENT,  // =
    // TOKEN_OP_EQUAL, // ==

    INFO("Start of some operator found - " << code[cursor]);

    switch (code[cursor]){
    case '+':
        eat();
        return Token(startTokenPos, TOKEN_OP_PLUS, code.substr(startTokenPos.cursor, 1));

    case '-':
        eat();
        return Token(startTokenPos, TOKEN_OP_MINUS, code.substr(startTokenPos.cursor, 1));

    case '*':
        eat();
        return Token(startTokenPos, TOKEN_OP_MUL, code.substr(startTokenPos.cursor, 1));

    case '/':
        eat();
        return Token(startTokenPos, TOKEN_OP_DIV, code.substr(startTokenPos.cursor, 1));

    case '%':
        eat();
        return Token(startTokenPos, TOKEN_OP_MOD, code.substr(startTokenPos.cursor, 1));

    case '=':
        if (matchPrefix("==")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_EQUAL, code.substr(startTokenPos.cursor, 2));
        } else {
            eat();
            return Token(startTokenPos, TOKEN_OP_ASSIGNMENT, code.substr(startTokenPos.cursor, 1));
        }

    case '>':
        if (matchPrefix(">=")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_GREATER_EQ, code.substr(startTokenPos.cursor, 2));
        } else {
            eat();
            return Token(startTokenPos, TOKEN_OP_GREATER, code.substr(startTokenPos.cursor, 1));
        }

    case '<':
        if (matchPrefix("<=")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_LESS_EQ, code.substr(startTokenPos.cursor, 2));
        } else {
            eat();
            return Token(startTokenPos, TOKEN_OP_LESS, code.substr(startTokenPos.cursor, 1));
        }

    case '!':
        if (matchPrefix("!=")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_NOT_EQUAL, code.substr(startTokenPos.cursor, 2));
        } else {
            eat();
            return Token(startTokenPos, TOKEN_OP_NOT, code.substr(startTokenPos.cursor, 1));
        }

    case '|':
        if (matchPrefix("||")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_OR, code.substr(startTokenPos.cursor, 2));
        } else {
            ERROR("Unrecognized operator: " << code[cursor] << " at " << printTokenPos());
        }

    case '&':
        if (matchPrefix("&&")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_AND, code.substr(startTokenPos.cursor, 2));
        } else {
            ERROR("Unrecognized operator: " << code[cursor] << " at " << printTokenPos());
        }

    default:
        ERROR("Unrecognized operator: " << code[cursor] << " at " << printTokenPos());
        break;
    }

}






// tokenizes soucrse code into tokens
Token MyTokenizer::nextToken(void) {


    handleSpaces(); // skip spaces and comments
    
    if (cursor < code.length()){
        
        updateStartTokenPos(); // current pos

        
        switch (lookup(code[cursor])){ // match token category by its first letter
        case WORD_START:
            return handleIdentifier();

        case DIGIT_LIT:
            return handleDigitLiteral();
        
        case CHAR_LIT:
            return handleCharLiteral();

        case STRING_LIT:
            return handleStringLiteral();

        case OPERATOR:
            return handleOperator();

        // single character tokens
        case SEMICOLON:
            INFO("Semicolom found  - ;");
            eat();
            return Token(startTokenPos, TOKEN_SEMICOLON, code.substr(startTokenPos.cursor, 1));

        case BRACE_OPEN:
            INFO("Brace open found  - {");
            eat();
            return Token(startTokenPos, TOKEN_BRACE_OPEN, code.substr(startTokenPos.cursor, 1));

        case BRACE_CLOSE:
            INFO("Brace close found  - }");
            eat();
            return Token(startTokenPos, TOKEN_BRACE_CLOSE, code.substr(startTokenPos.cursor, 1));
        
        case PARENTHESES_OPEN:
            INFO("Parentheses open found  - (");
            eat();
            return Token(startTokenPos, TOKEN_PARENTHESES_OPEN, code.substr(startTokenPos.cursor, 1));
    
        case PARENTHESES_CLOSE:
            INFO("Parentheses close found  - )");
            eat();
            return Token(startTokenPos, TOKEN_PARENTHESES_CLOSE, code.substr(startTokenPos.cursor, 1));
        
        case COMMA:
            INFO("Comma found  - ,");
            eat();
            return Token(startTokenPos, TOKEN_COMMA, code.substr(startTokenPos.cursor, 1));


        default:
            ERROR("Unrecognized token start:\n\t" << code[cursor] << "\nat " << printTokenPos());
            break;
        }



    } else{
        // end of code
        updateStartTokenPos();
        INFO("END OF FILE REACHED");
        return Token(startTokenPos, TOKEN_END, "EOF");
    } 
}