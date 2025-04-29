#pragma once


#include <vector>
#include "AST.hpp"



class IParser {
public:
    IParser() = delete;
    IParser(std::vector<Token>& Tokens){ (void)Tokens; };
    
    virtual ~IParser();

    virtual AST buildAST();
};


