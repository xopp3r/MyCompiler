#pragma once

#include "../common/nodes.hpp"
#include "IVisitor.hpp"

#include "scopeChecker.hpp"
// #include "functionCallChecker.hpp"
#include "controlFlowChecker.hpp"


class Validator : public IVisitor {
    public:
    Validator()
        : scopeChecker(ScopeChecker()), 
        //   functionCallChecker(FunctionCallChecker(scopeChecker.variableScopes)),
          controlFlowChecker(ControlFlowChecker(scopeChecker.variableScopes))
          {}

    void validate(Programm& root);

    private:    
    ScopeChecker scopeChecker;
    // FunctionCallChecker functionCallChecker;
    ControlFlowChecker controlFlowChecker;

    void enterScope(void);
    void leaveScope(void);
    void visitByAll(Node& node);
    void visitNonRecursivly(Node& node);
    

    void visit(Programm& node) override;
    void visit(Variable& node) override;
    void visit(CharLiteral& node) override;
    void visit(IfStatement& node) override;
    void visit(FunctionCall& node) override;
    void visit(StringLiteral& node) override;
    void visit(UnaryOperation& node) override;
    void visit(WhileStatement& node) override;
    void visit(IntegerLiteral& node) override;
    void visit(BinaryOperation& node) override;
    void visit(ReturnStatement& node) override;
    void visit(FunctionDefinition& node) override;
    void visit(ExpressionStatement& node) override;
    void visit(VariableDeclarationStatement& node) override;
};



void Validator::validate(Programm& root) {
    visit(root);
}

void Validator::visitByAll(Node& node) {
    node.accept(scopeChecker);
    // node.accept(functionCallChecker);
    node.accept(*this);
}

void Validator::visitNonRecursivly(Node& node) {
    node.accept(scopeChecker);
    // node.accept(functionCallChecker);
}


void Validator::enterScope(void) {
    scopeChecker.enterScope();
}

void Validator::leaveScope(void) {
    scopeChecker.leaveScope();
}







void Validator::visit(Programm& node) {
    enterScope();

    for (auto& var : node.globalVariables){
        visitByAll(*var);
    }

    for (auto& var : node.functions){
        visitByAll(*var);
    }

    leaveScope();
}

void Validator::visit(Variable& node) {
    (void)node;
}

void Validator::visit(CharLiteral& node) {
    (void)node;
}

void Validator::visit(IfStatement& node) {
    visitByAll(*node.condition);

    enterScope();
    for (auto& var : node.ifBody){
        visitByAll(*var);
    }
    leaveScope();

    enterScope();
    for (auto& var : node.elseBody){
        visitByAll(*var);
    }
    leaveScope();
}

void Validator::visit(FunctionCall& node) {

    for (auto& var : node.arguments){
        visitByAll(*var);
    }

}

void Validator::visit(StringLiteral& node) {
    (void)node;
}

void Validator::visit(UnaryOperation& node) {
    (void)node;
    // visitByAll(node);
}

void Validator::visit(WhileStatement& node) {
    visitByAll(*node.condition);

    enterScope();
    for (auto& var : node.body){
        visitByAll(*var);
    }
    leaveScope();
}

void Validator::visit(IntegerLiteral& node) {
    (void)node;
}

void Validator::visit(BinaryOperation& node) {
    (void)node;
    // visitByAll(node);
}

void Validator::visit(ReturnStatement& node) {
    (void)node;
}

void Validator::visit(FunctionDefinition& node) {
    enterScope();

    for (auto& var : node.arguments){
        visitByAll(*var);
    }

    for (auto& var : node.body){
        visitByAll(*var);
    }

    leaveScope();
}

void Validator::visit(ExpressionStatement& node) {
    visitByAll(node);
}

void Validator::visit(VariableDeclarationStatement& node) {
    visitByAll(node);
}
