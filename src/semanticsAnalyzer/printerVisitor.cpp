
#include "printerVisitor.hpp"
#include "../common/nodes.hpp"



#define SUBDIR(...) \
    prefix.resize(++depth + 1); \
    __VA_ARGS__ \
    prefix.resize(--depth + 1); \



void PrinterVisitor::PrintProgramASTasHierarcy(Programm& node){
    prefix.push_back(std::string_view(reinterpret_cast<const char*>(Nbar.data()))); // set first row to empty
    visit(node);
    depth = 0;
    prefix.clear();
    tempPrefixIndex = 0;
}


void PrinterVisitor::setTempPrefix(const std::u8string_view& tempPrefix, const std::u8string_view& expiritionPrefix){
    tempPrefixIndex = depth;
    afterPrefix = expiritionPrefix;
    prefix.at(depth) = std::string_view(reinterpret_cast<const char*>(tempPrefix.data()));
}

void PrinterVisitor::updateTempPrefix(){
    if (tempPrefixIndex != 0){ 
        prefix.at(tempPrefixIndex) = std::string_view(reinterpret_cast<const char*>(afterPrefix.data()));
        tempPrefixIndex = 0;
    }
}

void PrinterVisitor::print(const std::string_view& str){
    for (const auto& pref : prefix) std::cout << pref;
    std::cout << str << std::endl;

    updateTempPrefix();
}

void PrinterVisitor::visit(BinaryOperation& node){
    print(node.token.lexeme);
    SUBDIR(
        setTempPrefix(Cbar, Vbar);
        node.leftValue->accept(*this);
        setTempPrefix(Ebar, Nbar);
        node.rightValue->accept(*this);
    )
}

void PrinterVisitor::visit(UnaryOperation& node){
    print(node.token.lexeme);
    SUBDIR(
        setTempPrefix(Ebar, Nbar);
        node.value->accept(*this);
    )
}

void PrinterVisitor::visit(FunctionCall& node){
    print("FunctionCall");
    SUBDIR(
        setTempPrefix(Cbar, Vbar);
        print("adress");
        SUBDIR(
            setTempPrefix(Ebar, Nbar);
            node.functionAdress->accept(*this);
        )

        setTempPrefix(Ebar, Nbar);
        print("args " + std::to_string(node.arguments.size()));
        SUBDIR(
            printVec(node.arguments);
        )

    )
}

void PrinterVisitor::visit(IntegerLiteral& node){
    print("num(" + std::to_string(node.value) + ")");
}

void PrinterVisitor::visit(StringLiteral& node){
    print("str(" + std::string(node.value) + ")");
}

void PrinterVisitor::visit(CharLiteral& node){
    print("char(" + std::to_string(node.value) + ")");
}

void PrinterVisitor::visit(Variable& node){
    print("var(" + std::string(node.identifier) + ")");
}

void PrinterVisitor::visit(ExpressionStatement& node){
    print("Statement");
    SUBDIR(
        setTempPrefix(Ebar, Nbar);
        node.expression->accept(*this);
    )
}

void PrinterVisitor::visit(VariableDeclarationStatement& node){
    print(std::string(node.type.lexeme) + " " + std::string(node.name.lexeme));
}

void PrinterVisitor::visit(IfStatement& node){
    print("If");
    SUBDIR(            
        setTempPrefix(Cbar, Vbar);
        print("condition");
        SUBDIR(
            setTempPrefix(Ebar, Nbar);
            node.condition->accept(*this);
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

void PrinterVisitor::visit(WhileStatement& node){
    print("While");
    SUBDIR(            

        setTempPrefix(Cbar, Vbar);
        print("condition");
        SUBDIR(
            setTempPrefix(Ebar, Nbar);
            node.condition->accept(*this);
        )

        setTempPrefix(Ebar, Nbar);
        print("body " + std::to_string(node.body.size()));
        SUBDIR(
            printVec(node.body);
        )        
    )
}

void PrinterVisitor::visit(ReturnStatement& node){
    print("Return");
    SUBDIR(
        setTempPrefix(Ebar, Nbar);
        node.expression->accept(*this);
    )
}

void PrinterVisitor::visit(FunctionDefinition& node){
    print("Function " + std::string(node.returnType.lexeme) + " " + std::string(node.name.lexeme));
    SUBDIR(

        setTempPrefix(Cbar, Vbar);
        print("args " + std::to_string(node.arguments.size()));
        SUBDIR(
            printVec(node.arguments);
        )

        setTempPrefix(Ebar, Nbar);
        print("body " + std::to_string(node.body.size()));
            printVec(node.body);

    )
}

void PrinterVisitor::visit(Programm& node){
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

#undef SUBDIR
