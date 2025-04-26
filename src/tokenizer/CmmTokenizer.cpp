
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






// std::vector<std::regex> expressions = {
//     std::regex(R"([a-zA-Z_]\w*)"), // TOKEN_IDENTIFIER or TOKEN_KEYWORD
//     std::regex(R"(\d+)"),          // TOKEN_NUMBER
//     std::regex(R"('.')"),          // TOKEN_CHAR
//     std::regex(R"(".*")"),         // TOKEN_STRING
//     std::regex(R"()"),             // TOKEN_OPERATOR
// };





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

    size_t cursorStart = cursor;
    eat(); // opening quote
    
    auto matchingFunction = [](char c) -> bool {
        return c != 'c' and c != '\n';
    };

    while (matchingFunction(code[cursor]) and cursor < code.length()){
        eat();
    }

    if (cursor == code.length() or code[cursor] != '"') ERROR("Missing closing quote at " << printTokenPos());

    eat(); // closing quote


    std::string_view symbol = code.substr(cursorStart, cursor - cursorStart);

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
        return Token(startTokenPos, TOKEN_OP_PLUS, code.substr(cursor, 1));

    case '-':
        eat();
        return Token(startTokenPos, TOKEN_OP_MINUS, code.substr(cursor, 1));

    case '*':
        eat();
        return Token(startTokenPos, TOKEN_OP_MUL, code.substr(cursor, 1));

    case '/':
        eat();
        return Token(startTokenPos, TOKEN_OP_DIV, code.substr(cursor, 1));

    case '%':
        eat();
        return Token(startTokenPos, TOKEN_OP_MOD, code.substr(cursor, 1));

    case '=':
        if (matchPrefix("==")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_EQUAL, code.substr(cursor, 2));
        } else {
            eat();
            return Token(startTokenPos, TOKEN_OP_ASSIGNMENT, code.substr(cursor, 1));
        }

    case '>':
        if (matchPrefix(">=")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_GREATER_EQ, code.substr(cursor, 2));
        } else {
            eat();
            return Token(startTokenPos, TOKEN_OP_GREATER, code.substr(cursor, 1));
        }

    case '<':
        if (matchPrefix("<=")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_LESS_EQ, code.substr(cursor, 2));
        } else {
            eat();
            return Token(startTokenPos, TOKEN_OP_LESS, code.substr(cursor, 1));
        }

    case '!':
        if (matchPrefix("!=")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_NOT_EQUAL, code.substr(cursor, 2));
        } else {
            eat();
            return Token(startTokenPos, TOKEN_OP_NOT, code.substr(cursor, 1));
        }

    case '|':
        if (matchPrefix("||")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_OR, code.substr(cursor, 2));
        } else {
            ERROR("Unrecognized operator: " << code[cursor] << " at " << printTokenPos());
        }

    case '&':
        if (matchPrefix("&&")) {
            eat();
            eat();
            return Token(startTokenPos, TOKEN_OP_AND, code.substr(cursor, 2));
        } else {
            ERROR("Unrecognized operator: " << code[cursor] << " at " << printTokenPos());
        }

    default:
        ERROR("Unrecognized operator: " << code[cursor] << " at " << printTokenPos());
        break;
    }

}




// tokenizes soucrse code into tokens
Token MyTokenizer::nextToken() {


    
    if (cursor < code.length()){
        handleSpaces(); // skip spaces and comments
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


        default:
            ERROR("Unrecognized token start: " << code[cursor] << " at " << printTokenPos());
            break;
        }



    } else{
        // end of code
        updateStartTokenPos();
        return Token(startTokenPos, TOKEN_END, "EOF");
    } 
}