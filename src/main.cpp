
#include <iostream>
#include "tokenizer/tokenizer.hpp"


int main(int argc, char const *argv[]) {

    const char* sourceCode = 
" \ 
int main() { \ 
    return 69; \ 
} \ 
";
    
    MyTokenizer ctknzr(sourceCode);

    ctknzr.tokenize();

    return 0;
}
