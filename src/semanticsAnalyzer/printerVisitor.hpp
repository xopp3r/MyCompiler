#pragma once 

#include "IVisitor.hpp"

#include <string>
#include <memory>
#include <vector>



constexpr auto Vbar = std::u8string_view((u8"│ "));  
constexpr auto Cbar = std::u8string_view((u8"├─"));
constexpr auto Ebar = std::u8string_view((u8"└─"));
constexpr auto Nbar = std::u8string_view((u8"  "));



class PrinterVisitor final : public IVisitor {
    public:
    void PrintProgramASTasHierarcy(Programm& node);

    private:
    size_t depth = 0;
    std::vector<std::string_view> prefix;
    size_t tempPrefixIndex = 0;
    std::u8string_view afterPrefix;

    void updateTempPrefix();
    void setTempPrefix(const std::u8string_view& tempPrefix, const std::u8string_view& expiritionPrefix);
    
    void print(const std::string_view& str);
    void printVec(const std::vector<std::unique_ptr<Node>>& vec);

    void visit(BinaryOperation& node) override;
    void visit(UnaryOperation& node) override;
    void visit(FunctionCall& node) override;
    void visit(IntegerLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(CharLiteral& node) override;
    void visit(Variable& node) override;
    void visit(ExpressionStatement& node) override;
    void visit(VariableDeclarationStatement& node) override;
    void visit(IfStatement& node) override;
    void visit(WhileStatement& node) override;
    void visit(ReturnStatement& node) override;
    void visit(FunctionDefinition& node) override;
    void visit(Programm& node) override;


    template<typename T>
    requires std::derived_from<T, Node> 
    void printVec(const std::vector<std::unique_ptr<T>>& vec) {
        if (vec.empty()) return;

        // SUBDIR( 
        prefix.resize(++depth + 1); \

            for (size_t i = 0; i < vec.size() - 1; ++i) {
                setTempPrefix(Cbar, Vbar);
                vec[i]->accept(*this);
            }
            
            setTempPrefix(Ebar, Nbar);
            vec.back()->accept(*this);

        prefix.resize(--depth + 1); \
        // )
    }

};


