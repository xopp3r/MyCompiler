#pragma once


#include "nodes.hpp"



class AST {
public:
    AST() = delete;
    AST(std::unique_ptr<Node> Root)
        : root(std::move(Root)) {};
    ~AST() = default;

    std::unique_ptr<Node> root;
};

