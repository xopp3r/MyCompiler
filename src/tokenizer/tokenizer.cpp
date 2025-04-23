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



inline bool isIdentifier(char c){
    return isalnum(c) || c == '_';
}



bool MyTokenizer::matchPrefix(std::string_view prefix){
    
    if (cursor + prefix.length() >= code.length()) 
        return false;

    if (code.compare(cursor, prefix.length(), prefix) == 0) 
        return true;

    return false;
}



bool matchType(std::string_view str, std::vector<std::string_view> wordClass){

    for (int i = 0; i < wordClass.size(); i++) {
        if (str.compare(wordClass[i])) return true;
    }
    
    return false;
}





MyTokenizer::MyTokenizer(std::string_view code)
    : code(code), ITokenizer(code) {}



void MyTokenizer::eat(size_t len){

    for (size_t i = 0; i < len; i++) {
     
        if (code[cursor] == '\n'){ // если дошли до конца строки
            cursor++; 
            lineNumber++;
            lineBegining = cursor;
        } else {
            cursor++; // сдвигаем курсор
        }

    }
}


std::string_view MyTokenizer::getSequence(std::function<bool(char)> matchingFunction){

    size_t cursorStart = cursor;
    
    while (cursor < code.length() && matchingFunction(code[cursor])){
        eat(1);
    }

    return std::string_view(code.data() + cursorStart, cursor - cursorStart);
}




std::vector<Token> MyTokenizer::tokenize() {
    

    
    
    while (cursor < code.size()){
        
        // если токен - символ (переменная/keyword/тип)
        if (isIdentifier(code[cursor]) && !isdigit(code[cursor])){
            
            std::string_view symbol = getSequence(isIdentifier);
            
            if (matchType(symbol, keywords)){
                
                INFO << "Keyword found - " << symbol << std::endl;
                tokens.push_back(Token(symbol.length(), cursor, TOKEN_KEYWORD, symbol));
                
            } else {
                
                INFO << "Identifier found - " << symbol << std::endl;
                tokens.push_back(Token(symbol.length(), cursor, TOKEN_SYMBOL, symbol));
                
            }
            
        }


        // если число
        if (isdigit(code[cursor])){
            
            std::string_view symbol = getSequence([](char c) -> bool { 
                return isdigit(c); 
            });

            INFO << "Number literal found - " << symbol << std::endl;
            tokens.push_back(Token(symbol.length(), cursor, TOKEN_NUMBER, symbol));
            
        }


        // если строка
        if (code[cursor] == '"'){

            eat(1);

            std::string_view symbol = getSequence([this](char c) -> bool {

                if (c == '\n')
                    ERROR("Not closed string literal at " << lineNumber << " row ");

                return c != '"';

            });
            
            eat(1);

            INFO << "String literal found - " << symbol << std::endl;
            tokens.push_back(Token(symbol.length(), cursor, TOKEN_STRING, symbol));
            
        }

        
        // если буква
        if (code[cursor] == 39){ // ascii(39) the single quote ( ' )

            eat(1);

            std::string_view symbol = getSequence([this](char c) -> bool {

                if (c == '\n')
                    ERROR("Not closed string literal at " << lineNumber << " row ");

                return c != 39;
            });

            eat(1);

            if (symbol.length() != 1) 
                ERROR("Invalid char literal at " << lineNumber << " row " << cursor - lineBegining << " column");

            INFO << "Char literal found - " << symbol << std::endl;
            tokens.push_back(Token(symbol.length(), cursor, TOKEN_CHAR, symbol));
            
        }

        // если пунктуация
        if (matchPrefix("//")){

            (void)getSequence([](char c) -> bool {
                return c != '\n';
            });

        }


        // если операторы
        if (matchPrefix("//")){

            (void)getSequence([](char c) -> bool {
                return c != '\n';
            });

        }

        
        // если комментарий
        if (matchPrefix("//")){

            (void)getSequence([](char c) -> bool {
                return c != '\n';
            });

        }
        

        // пропускаем пробелы
        if (isspace(code[cursor])){

            (void)getSequence([](char c) -> bool { 
                return isspace(c); 
            });

        }
        
    }
    
    tokens.push_back(Token(0, code.size(), TOKEN_END, "\0"));
}