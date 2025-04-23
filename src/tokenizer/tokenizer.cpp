#include "tokenizer.hpp"
#include "../logger.h"

#include <ctype.h>
#include <assert.h>
#include <array>
#include <stdexcept>





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
    "~", "&", "|", "^", "<<", ">>", // "<<<", ">>>", 
    "=" // "+-*=...."
};


std::vector<std::string_view> punctuators = {
    "(){}[],"
};







MyTokenizer::MyTokenizer(std::string_view code)
    : code(code), ITokenizer(code) {}



bool MyTokenizer::matchPrefix(std::string_view prefix){
    
    if (cursor + prefix.length() >= code.length()) 
        return false;

    if (code.compare(cursor, prefix.length(), prefix) == 0) 
        return true;

    return false;
}



bool matchType(std::string_view str, std::vector<std::string_view> words){

    for (int i = 0; i < words.size(); i++) {
        if (str.compare(words[i])) return true;
    }
    
    return false;
}



// void MyTokenizer::eat(size_t len){

//     for (size_t i = 0; i < len; i++) {
     
//         if (code[cursor] == '\n'){ // если дошли до конца строки
//             cursor++; 
//             lineNumber++;
//             lineBegining = cursor;
//         } else {
//             cursor++; // сдвигаем курсор
//         }

//     }
// }

void MyTokenizer::eat(void){
    if (code[cursor] == '\n'){ // если дошли до конца строки
        cursor++; 
        lineNumber++;
        lineBegining = cursor;
    } else {
        cursor++; // сдвигаем курсор
    }
}




void MyTokenizer::handleIdentifier(void){

    std::string_view symbol = getSequence([](char c){
        return isalnum(c) or c == '_';
    });
    

    if (matchType(symbol, keywords)){
        
        INFO << "Keyword found - " << symbol << std::endl;
        tokens.push_back(Token(symbol.length(), cursor, TOKEN_KEYWORD, symbol));
        
    } else {
        
        INFO << "Identifier found - " << symbol << std::endl;
        tokens.push_back(Token(symbol.length(), cursor, TOKEN_SYMBOL, symbol));
        
    }

}



void MyTokenizer::handleDigitLiteral(void){

    std::string_view symbol = getSequence([](char c) -> bool { 
        return isdigit(c); 
    });

    INFO << "Number literal found - " << symbol << std::endl;
    tokens.push_back(Token(symbol.length(), cursor, TOKEN_NUMBER, symbol));

}



void MyTokenizer::handleStringLiteral(void){

    eat();

    std::string_view symbol = getSequence([this](char c) -> bool {
        return c != '"';
    });

    if (code[cursor] == code.length() or code[cursor] != '"') ERROR("Missing closing quote at line " << lineNumber);

    eat();

    INFO << "String literal found - " << symbol << std::endl;
    tokens.push_back(Token(symbol.length(), cursor, TOKEN_STRING, symbol));

}



void MyTokenizer::handleCharLiteral(void){

    eat(); // opening quote

    std::string_view symbol = getSequence([this](char c) -> bool {
        return c != 39; // ascii(39) the single quote ( ' )
    });

    if (code[cursor] == code.length() or code[cursor] != 39) ERROR("Missing closing quote at line " << lineNumber);

    eat(); // closing quote

    if (symbol.length() != 1) ERROR("Invalid char literal at " << lineNumber << " row " << cursor - lineBegining << " column");

    INFO << "Char literal found - " << symbol << std::endl;
    tokens.push_back(Token(symbol.length(), cursor, TOKEN_CHAR, symbol));

}



// tokenizes soucrse code into tokens
std::vector<Token> MyTokenizer::tokenize() {
    
    
    while (cursor < code.size()){
        
        // if token is an identifier
        if (isalpha(code[cursor]) or code[cursor] == '_'){
            handleIdentifier();
            continue;
        }

        // number literal
        if (isdigit(code[cursor])){
            handleDigitLiteral();
            continue;
        }

        // string literal
        if (code[cursor] == '"'){
            handleStringLiteral();
            continue;
        }
        
        // char literal
        if (code[cursor] == 39){ // ascii(39) the single quote ( ' )
            handleCharLiteral();
            continue;
        }

        // punctuation
        if (matchPrefix("//")){

            (void)getSequence([](char c) -> bool {
                return c != '\n';
            });

            continue;
        }


        // operator
        if (matchPrefix("//")){

            (void)getSequence([](char c) -> bool {
                return c != '\n';
            });

            continue;
        }

        
        // comment
        if (matchPrefix("//")){
            (void)getSequence([](char c) -> bool {
                return c != '\n';
            });
            continue;
        }
        

        // skip spaces
        if (isspace(code[cursor])){
            (void)getSequence([](char c) -> bool { 
                return isspace(c); 
            });
            continue;
        }
        

        ERROR("Unreconnized token (" << code[cursor] << ") at row " << lineNumber << " column " << cursor - lineBegining);
    }
    
    // end of code token
    tokens.push_back(Token(0, code.size(), TOKEN_END, "\0"));
}