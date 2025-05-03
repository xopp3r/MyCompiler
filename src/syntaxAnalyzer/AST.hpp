#pragma once


#include "nodes.hpp"



class AST {
public:
    AST() = delete;
    AST(std::unique_ptr<Programm> Root)
        : root(std::move(Root)) {};
    ~AST() = default;

    std::unique_ptr<Programm> root;
};

