#pragma once

#include "ITokenizer.hpp"


// Cmm tokenizer
class MyTokenizer : public ITokenizer {
    public:
    MyTokenizer() = delete;
    ~MyTokenizer();
    MyTokenizer(std::string_view code) 
        : code(code), ITokenizer(code) {}
    
        
    // returns next token
    Token nextToken() override;

    
    private:
    // source code
    std::string_view code;

    // cursor stuff
    size_t lineNumber; 
    size_t cursor; // current index
    size_t lineBeginingCursor; // index of line start
    
    // positioning stuff
    Position startTokenPos; // pos of start of current token
    void updateStartTokenPos(void);
    std::string printTokenPos(void);

    // tokenizing
    char eat(void); // consume 1 char moving cursor
    bool matchPrefix(std::string_view& prefix); 
    
    // token handling
    Token handleCharLiteral(void);
    Token handleStringLiteral(void);
    Token handleIdentifier(void);
    Token handleDigitLiteral(void);
    void  handleSpaces(void);
    // TODO: other types

    // XDD
    template<typename Matcher> requires std::invocable<Matcher, char>
    std::string_view getSequence(Matcher&& matchingFunction) {
        size_t cursorStart = cursor;
        while (matchingFunction(code[cursor]) and cursor < code.length()){
            eat();
        }
        return code.substr(cursorStart, cursor - cursorStart);
    }
};
