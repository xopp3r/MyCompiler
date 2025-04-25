
#include "CmmTokenizer.hpp"
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


std::vector<std::string_view> operators = {
    "+", "-", "*", "/", "%", "++", "--", 
    "==", ">", "<", ">=", "<=", "!=",
    "&&", "||", "!",
    // "~", "&", "|", "^", "<<", ">>", // "<<<", ">>>", 
    "=" // "+-*=...."
};


std::vector<std::string_view> punctuators = {
    "(){}[],"
};





// std::vector<std::regex> expressions = {
//     std::regex(R"([a-zA-Z_]\w*)"), // TOKEN_IDENTIFIER or TOKEN_KEYWORD
//     std::regex(R"(\d+)"),          // TOKEN_NUMBER
//     std::regex(R"('.')"),          // TOKEN_CHAR
//     std::regex(R"(".*")"),         // TOKEN_STRING
//     std::regex(R"()"),             // TOKEN_OPERATOR
// };





bool MyTokenizer::matchPrefix(std::string_view& prefix){
    
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
    (void) getSequence([](char c) -> bool {
        return isspace(c);
    });
}

 




Token MyTokenizer::handleIdentifier(void){
    
    std::string_view symbol = getSequence([](char c) -> bool {
        return isalnum(c) or c == '_';
    });
    

    if (matchType(symbol, keywords)){
        
        INFO << "Keyword found - " << symbol << std::endl;
        return Token(startTokenPos, TOKEN_KEYWORD, symbol);
        
    } else {
        
        INFO << "Identifier found - " << symbol << std::endl;
        return Token(startTokenPos, TOKEN_IDENTIFIER, symbol);
        
    }

}



Token MyTokenizer::handleDigitLiteral(void){

    std::string_view symbol = getSequence([](char c) -> bool { 
        return isdigit(c); 
    });

    INFO << "Number literal found - " << symbol << std::endl;
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

    if (code[cursor] == code.length() or code[cursor] != '"') ERROR("Missing closing quote at " << printTokenPos());

    eat(); // closing quote


    std::string_view symbol = code.substr(cursorStart, cursor - cursorStart);

    INFO << "String literal found - " << symbol << std::endl;
    return Token(startTokenPos, TOKEN_STRING, symbol);
}



Token MyTokenizer::handleCharLiteral(void){

    eat(); // opening quote

    std::string_view symbol = getSequence([](char c) -> bool {
        return c != 39; // ascii(39) the single quote ( ' )
    });

    if (code[cursor] == code.length() or code[cursor] != 39) ERROR("Missing closing quote at " << printTokenPos());

    eat(); // closing quote

    if (symbol.length() != 1) ERROR("Invalid char literal at " << printTokenPos());

    INFO << "Char literal found - " << symbol << std::endl;
    return Token(startTokenPos, TOKEN_CHAR, symbol);
}



// tokenizes soucrse code into tokens
Token MyTokenizer::nextToken() {


    
    if (cursor < code.length()){
        handleSpaces();
        updateStartTokenPos();
  
        
        // TODO: IMPLEMENTATION   
        switch ()
        {
        case 'sth':
            /* code */
            break;
        
        default:
            break;
        }



    } else{
        // end of code
        updateStartTokenPos();
        return Token(startTokenPos, TOKEN_END, "\0");
    } 
}