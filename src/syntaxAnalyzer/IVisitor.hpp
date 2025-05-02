#pragma once

#include "nodes.hpp"



// interface for visitor pattern
class IVisitor {
public:
    virtual ~IVisitor() = default;

    virtual void visit(BinaryOperation& node) = 0;
    virtual void visit(UnaryOperation& node) = 0;
    virtual void visit(FunctionCall& node) = 0;
    virtual void visit(IntegerLiteral& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(CharLiteral& node) = 0;
    virtual void visit(Variable& node) = 0;
    virtual void visit(ExpressionStatement& node) = 0;
    virtual void visit(VariableDeclarationStatement& node) = 0;
    virtual void visit(IfStatement& node) = 0;
    virtual void visit(WhileStatement& node) = 0;
    virtual void visit(ReturnStatement& node) = 0;
    virtual void visit(FunctionDefinition& node) = 0;
    virtual void visit(Programm& node) = 0;

};



