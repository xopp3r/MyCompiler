#pragma once

#include "IParser.hpp"


class MyParser : public IParser {
    public:
    MyParser() = delete;
    ~MyParser() = default;
    MyParser(std::vector<Token>& Tokens);

    AST buildAST() override;

    private:
    size_t cursor = 0;
    std::vector<Token>& tokens;
};



