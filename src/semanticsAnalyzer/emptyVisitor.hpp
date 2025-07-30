#pragma once

#include "IVisitor.hpp"



class EmptyVisitor : public IVisitor {
    public:
    virtual ~EmptyVisitor() = default;

    void visit(Programm& node) override { (void)node; }
    void visit(Variable& node) override { (void)node; }
    void visit(CharLiteral& node) override { (void)node; }
    void visit(IfStatement& node) override { (void)node; }
    void visit(FunctionCall& node) override { (void)node; }
    void visit(StringLiteral& node) override { (void)node; }
    void visit(UnaryOperation& node) override { (void)node; }
    void visit(WhileStatement& node) override { (void)node; }
    void visit(IntegerLiteral& node) override { (void)node; }
    void visit(BinaryOperation& node) override { (void)node; }
    void visit(ReturnStatement& node) override { (void)node; }
    void visit(FunctionDefinition& node) override { (void)node; }
    void visit(ExpressionStatement& node) override { (void)node; }
    void visit(VariableDeclarationStatement& node) override { (void)node; }

    private:
};

