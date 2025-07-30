#pragma once

#include "emptyVisitor.hpp"
#include "../common/token.hpp"
#include "../common/types.hpp"
#include <unordered_map>
#include <vector>

using Scopes = std::vector<std::unordered_map<std::string_view, PrimitiveType>>;


class ScopeChecker final : public EmptyVisitor {
    public:
    void enterScope(void);
    void leaveScope(void);


    void visit(Variable& node) override;
    void visit(FunctionDefinition& node) override;
    void visit(VariableDeclarationStatement& node) override;

    
    Scopes variableScopes;
    
    private:
};

