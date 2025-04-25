
#include <iostream>
#include <vector>

#include "tokenizer/CmmTokenizer.hpp"


int main(int argc, char const *argv[]) {

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

    return 0;
}
