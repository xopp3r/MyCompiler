#pragma once

#include "scopeChecker.hpp"



class ControlFlowChecker final : public EmptyVisitor {
    public:
    ControlFlowChecker(Scopes& scopes) : variableScopes(scopes) {};

    Scopes& variableScopes;

    void visit(FunctionDefinition& node) override;

    private:
};

