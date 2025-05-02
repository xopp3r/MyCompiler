#pragma once 

#include <array>
#include <stdexcept>




enum TokenCategory {
    UNDEFINED = 0,
    WORD_START,
    DIGIT_LIT,
    CHAR_LIT,
    STRING_LIT,
    OPERATOR,
    SEMICOLON,
    COLON,
    SQR_BRACE_OPEN,
    SQR_BRACE_CLOSE,
    BRACE_OPEN,
    BRACE_CLOSE,
    PARENTHESES_OPEN,
    PARENTHESES_CLOSE,
    COMMA,
    END
};







static constexpr std::array<TokenCategory, 256> generateLookupTable(){
    std::array<TokenCategory, 256> arr{};
    
    // literals
    arr[39 ] = CHAR_LIT;   // ascii(39) = '
    arr['"'] = STRING_LIT;
    
    for (char c = '0'; c <= '9'; c++){
        arr[c] = DIGIT_LIT;
    }

    arr[';'] = SEMICOLON;
    arr[')'] = PARENTHESES_CLOSE;
    arr['('] = PARENTHESES_OPEN;
    arr['}'] = BRACE_CLOSE;
    arr['{'] = BRACE_OPEN;
    arr[']'] = SQR_BRACE_CLOSE;
    arr['['] = SQR_BRACE_OPEN;
    arr[','] = COMMA;
    arr[':'] = COLON;
     

    // operators
    for (char c : R"(=+-/%<>!&*|@)"){
        arr[c] = OPERATOR;
    }
    

    // identifiers
    arr['_'] = WORD_START;

    for (char c = 'a'; c <= 'z'; c++){
        arr[c] = WORD_START;
    }

    for (char c = 'A'; c <= 'Z'; c++){
        arr[c] = WORD_START;
    }

    // end
    arr[0] = END;

    return arr;
}





static constexpr auto lookupTable = generateLookupTable();




static constexpr TokenCategory lookup(unsigned char character){
    if (character >= lookupTable.size()){
        throw std::out_of_range("Lookup table index is out of bounds (> 255)");
    } 
    return lookupTable[character];
}


