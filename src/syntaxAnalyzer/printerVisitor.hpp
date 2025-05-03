#pragma once 


#include "IVisitor.hpp"
#include "nodes.hpp"




class PrinterVisitor final : public IVisitor {
    public:
    void print(Programm& node){
        visit(node);
        deep = 0;
    }

    private:
    size_t deep = 0;

    void printLine(std::string_view str){
        std::cout << std::string(deep*2, ' ') << str << std::endl;
    }



    void visit(BinaryOperation& node){
        printLine(tokenName(node.operation));
        deep++;
            node.leftValue->accept(this);
            node.rightValue->accept(this);
        deep--;
    }

    void visit(UnaryOperation& node){
        printLine(tokenName(node.operation));
        deep++;
            node.value->accept(this);
        deep--;
    }

    void visit(FunctionCall& node){
        printLine("FunctionCall");
        deep++;

            printLine("adress");
            deep++;
                node.functionAdress->accept(this);
            deep--;

            printLine("args " + std::to_string(node.arguments.size()));
            deep++;
                for (const auto& expr : node.arguments) expr->accept(this);
            deep--;

        deep--;
    }

    void visit(IntegerLiteral& node){
        printLine("num(" + std::to_string(node.value) + ")");
    }

    void visit(StringLiteral& node){
        printLine("str(" + node.value + ")");
    }

    void visit(CharLiteral& node){
        printLine("char(" + std::to_string(node.value) + ")");
    }

    void visit(Variable& node){
        printLine("var(" + std::string(node.token.lexeme) + ")");
    }

    void visit(ExpressionStatement& node){
        printLine("Statement");
        deep++;
            node.expression->accept(this);
        deep--;
    }

    void visit(VariableDeclarationStatement& node){
        printLine(std::string(node.type.lexeme) + " " + std::string(node.name.lexeme));
    }

    void visit(IfStatement& node){
        printLine("If");
        deep++;
            
            printLine("condition");
            deep++;
                node.condition->accept(this);
            deep--;

            printLine("ifBody " + std::to_string(node.ifBody.size()));
            deep++;
                for (const auto& st : node.ifBody) st->accept(this);
            deep--;
        
            printLine("elseBody " + std::to_string(node.elseBody.size()));
            deep++;
                for (const auto& st : node.elseBody) st->accept(this);
            deep--;

        deep--;
    }

    void visit(WhileStatement& node){
        printLine("While");
        deep++;
            
            printLine("condition");
            deep++;
                node.condition->accept(this);
            deep--;

            printLine("body " + std::to_string(node.body.size()));
            deep++;
                for (const auto& st : node.body) st->accept(this);
            deep--;
        
        deep--;
    }

    void visit(ReturnStatement& node){
        printLine("Return");
        deep++;
            node.expression->accept(this);
        deep--;
    }

    void visit(FunctionDefinition& node){
        printLine("Function " + std::string(node.returnType.lexeme) + " " + std::string(node.name.lexeme));
        deep++;

            printLine("args " + std::to_string(node.arguments.size()));
            deep++;
                for (const auto& arg : node.arguments) printLine(std::string(arg.first.lexeme) + " " + std::string(arg.second.lexeme));
            deep--;

            printLine("body " + std::to_string(node.body.size()));
            deep++;
                for (const auto& st : node.body) st->accept(this);
            deep--;

        deep--;
    }

    void visit(Programm& node){
        printLine("Programm");
        deep++;

            printLine("Globals " + std::to_string(node.globalVariables.size()));
            deep++;
                for (const auto& var : node.globalVariables) var->accept(this);
            deep--;

            printLine("Functions " + std::to_string(node.functions.size()));
            deep++;
                for (const auto& func : node.functions) func->accept(this);
            deep--;

        deep--;
    }



};

