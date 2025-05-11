#pragma once


#include <functional>
#include "../common/AST.hpp"



class IParser {
public:
    IParser() = delete;
    IParser(std::function<Token(void)> nextTokenCallback){ (void)nextTokenCallback; };
    
    virtual ~IParser() = default;

    virtual AST buildAST() = 0;
};


