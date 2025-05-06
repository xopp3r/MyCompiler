#pragma once 


#include "IVisitor.hpp"
#include "nodes.hpp"

#include <string>
#include <vector>


constexpr auto Vbar = std::u8string_view((u8"│ "));  
constexpr auto Cbar = std::u8string_view((u8"├─"));
constexpr auto Ebar = std::u8string_view((u8"└─"));
constexpr auto Nbar = std::u8string_view((u8"  "));


#define SUBDIR(...) \
    prefix.resize(++depth + 1); \
    __VA_ARGS__ \
    prefix.resize(--depth + 1); \





class PrinterVisitor final : public IVisitor {
    public:
    void PrintProgramASTasHierarcy(Programm* node){
        prefix.push_back(std::string_view(reinterpret_cast<const char*>(Nbar.data()))); // set first row to empty
        visit(*node);
        depth = 0;
        prefix.clear();
        tempPrefixIndex = 0;
    }


    private:
    size_t depth = 0;
    std::vector<std::string_view> prefix;

    size_t tempPrefixIndex = 0;
    std::u8string_view afterPrefix;


    void setTempPrefix(const std::u8string_view& tempPrefix, const std::u8string_view& expiritionPrefix){
        tempPrefixIndex = depth;
        afterPrefix = expiritionPrefix;
        prefix.at(depth) = std::string_view(reinterpret_cast<const char*>(tempPrefix.data()));
    }

    void updateTempPrefix(){
        if (tempPrefixIndex != 0){ 
            prefix.at(tempPrefixIndex) = std::string_view(reinterpret_cast<const char*>(afterPrefix.data()));
            tempPrefixIndex = 0;
        }
    }


    void print(const std::string_view& str){
        for (const auto& pref : prefix) std::cout << pref;
        std::cout << str << std::endl;

        updateTempPrefix();
    }


    template <typename T>
    void printVec(const std::vector<T>& vec) {
        if (vec.empty()) return;

        SUBDIR(

            for (size_t i = 0; i < vec.size() - 1; ++i) {
                setTempPrefix(Cbar, Vbar);
                vec[i]->accept(this);
            }
            
            setTempPrefix(Ebar, Nbar);
            vec.back()->accept(this);
        
        )
    }


    // void printLn(const std::string_view& str, const std::string_view& newPrefix = ""){

    //     setPrefix(newPrefix);
    //     print(str);

    // }






    void visit(BinaryOperation& node){
        print(tokenName(node.operation));
        SUBDIR(
            setTempPrefix(Cbar, Vbar);
            node.leftValue->accept(this);
            setTempPrefix(Ebar, Nbar);
            node.rightValue->accept(this);
        )
    }

    void visit(UnaryOperation& node){
        print(tokenName(node.operation));
        SUBDIR(
            setTempPrefix(Ebar, Nbar);
            node.value->accept(this);
        )
    }

    void visit(FunctionCall& node){
        print("FunctionCall");
        SUBDIR(
            setTempPrefix(Cbar, Vbar);
            print("adress");
            SUBDIR(
                setTempPrefix(Ebar, Nbar);
                node.functionAdress->accept(this);
            )

            setTempPrefix(Ebar, Nbar);
            print("args " + std::to_string(node.arguments.size()));
            SUBDIR(
                printVec(node.arguments);
            )

        )
    }

    void visit(IntegerLiteral& node){
        print("num(" + std::to_string(node.value) + ")");
    }

    void visit(StringLiteral& node){
        print("str(" + node.value + ")");
    }

    void visit(CharLiteral& node){
        print("char(" + std::to_string(node.value) + ")");
    }

    void visit(Variable& node){
        print("var(" + std::string(node.token.lexeme) + ")");
    }

    void visit(ExpressionStatement& node){
        print("Statement");
        SUBDIR(
            setTempPrefix(Ebar, Nbar);
            node.expression->accept(this);
        )
    }

    void visit(VariableDeclarationStatement& node){
        print(std::string(node.type.lexeme) + " " + std::string(node.name.lexeme));
    }

    void visit(IfStatement& node){
        print("If");
        SUBDIR(
            
            setTempPrefix(Cbar, Vbar);
            print("condition");
            SUBDIR(
                setTempPrefix(Ebar, Nbar);
                node.condition->accept(this);
            )

            setTempPrefix(Cbar, Vbar);
            print("ifBody " + std::to_string(node.ifBody.size()));
            SUBDIR(
                printVec(node.ifBody);
            )
        
            setTempPrefix(Ebar, Nbar);
            print("elseBody " + std::to_string(node.elseBody.size()));
            SUBDIR(
                printVec(node.elseBody);
            )

        )
    }

    void visit(WhileStatement& node){
        print("While");
        SUBDIR(
            

            setTempPrefix(Cbar, Vbar);
            print("condition");
            SUBDIR(
                setTempPrefix(Ebar, Nbar);
                node.condition->accept(this);
            )

            setTempPrefix(Ebar, Nbar);
            print("body " + std::to_string(node.body.size()));
            SUBDIR(
                printVec(node.body);
            )
        
        )
    }

    void visit(ReturnStatement& node){
        print("Return");
        SUBDIR(
            setTempPrefix(Ebar, Nbar);
            node.expression->accept(this);
        )
    }

    void visit(FunctionDefinition& node){
        print("Function " + std::string(node.returnType.lexeme) + " " + std::string(node.name.lexeme));
        SUBDIR(

            setTempPrefix(Cbar, Vbar);
            print("args " + std::to_string(node.arguments.size()));
            SUBDIR(
                if (node.arguments.size() > 0) {
                    for (size_t arg = 0; arg + 1 < node.arguments.size(); arg++){
                        setTempPrefix(Cbar, Vbar);
                        print(std::string(node.arguments[arg].first.lexeme) + " " + std::string(node.arguments[arg].second.lexeme));
                    }

                    setTempPrefix(Ebar, Nbar);
                    print(std::string(node.arguments.back().first.lexeme) + " " + std::string(node.arguments.back().second.lexeme));
                }
            )

            setTempPrefix(Ebar, Nbar);
            print("body " + std::to_string(node.body.size()));
                printVec(node.body);

        )
    }


    void visit(Programm& node){
        print("Programm");
        SUBDIR(

            setTempPrefix(Cbar, Vbar);
            print("Globals " + std::to_string(node.globalVariables.size()));
                printVec(node.globalVariables);

            setTempPrefix(Ebar, Nbar);
            print("Functions " + std::to_string(node.functions.size()));
                printVec(node.functions);

        )

    }



};


#undef SUBDIR
