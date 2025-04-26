
#include <iostream>
#include <vector>

#include "tokenizer/CmmTokenizer.hpp"


int main(void){  //int argc, char const *argv[]

    const char* sourceCode =
" \
int main() { \
    return 69; \
} \
";
    
    MyTokenizer ctknzr(sourceCode);
    std::vector<Token> tokens;

    for (Token t = ctknzr.nextToken(); t.type != TOKEN_END; t = ctknzr.nextToken()) {
        tokens.push_back(t);
    }

    for (const auto& token : tokens) {
        std::cout << token.lexeme << std::endl;
    }
    

    return 0;
}
