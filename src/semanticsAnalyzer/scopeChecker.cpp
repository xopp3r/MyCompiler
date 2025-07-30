
#include "scopeChecker.hpp"
#include "../common/nodes.hpp"



void ScopeChecker::enterScope(void){
    variableScopes.resize(variableScopes.size() + 1);
}


void ScopeChecker::leaveScope(void){
    variableScopes.resize(variableScopes.size() - 1);
}


void ScopeChecker::visit(Variable& node) {
    for (auto it = variableScopes.rbegin(); it != variableScopes.rend(); ++it) {
        if (it->find(node.identifier) != it->end()) return;
    }

    ERROR("Undefined symbol");
}


void ScopeChecker::visit(FunctionDefinition& node) {
    auto result = variableScopes.back().try_emplace(node.name.lexeme, getType(node.returnType.lexeme));
    if (not result.second) ERROR("Multiple defenitions of variable");
}


void ScopeChecker::visit(VariableDeclarationStatement& node) {
    auto result = variableScopes.back().try_emplace(node.name.lexeme, getType(node.type.lexeme));
    if (not result.second) ERROR("Multiple defenitions of variable");
}

