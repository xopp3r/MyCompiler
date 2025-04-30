#pragma once



#include <memory>
#include "../tokenizer/token.hpp"
#include "../common/logger.hpp"




class Node {
    public:
        virtual ~Node() = default;
        
        // virtual method for Visitor pattern
        // virtual void accept(class Visitor& visitor) const = 0;
        
        // Prevent slicing
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        
        // Allow moving
        Node(Node&&) = default;
        Node& operator=(Node&&) = default;
    
    protected:
        Node() = default; 
    };






// class OOOOOOOO : public Node {
//     public:
//     OOOOOOOO(Token nodeToken) : Node(nodeToken) {};

// };







// < ================ EXPRESSIONS ================ >

class Expression : public Node {
    public:
    virtual ~Expression() override = default;

    protected:
    Expression() = default;
};







// < ================ LITERALS ================ >


class Literal : public Expression{
    public:
    Literal(Token nodeToken) : token(nodeToken) {};

    ~Literal() override = default;
    
    Token token;
};


    class IntegerLiteral final : public Literal {
        public:
        IntegerLiteral(Token nodeToken) 
            : Literal(nodeToken) {
            
            try {
                value = std::stoi(token.lexeme.data()); // string to int
            } catch (std::invalid_argument const& ex) {
                ERROR("Invalid number literal at " << token.position.string() << "\n");
            } catch (std::out_of_range const& ex) {
                ERROR("Number literal at " << token.position.string() << " is too big for int type\n");
            }

        };
        

        int value;
        
    };


    class StringLiteral final : public Literal {
        public:
        StringLiteral(Token nodeToken) 
            : Literal(nodeToken), value(nodeToken.lexeme) {};
        
        std::string value;
        
    };


    class CharLiteral final : public Literal {
        public:
        CharLiteral(Token nodeToken) 
            : Literal(nodeToken), value(nodeToken.lexeme.at(0)) {};
        
        char value;
        
    };
        
    


















// < ================ STATEMENTS ================ >

class Statement : public Node {
    
};
    // TODO: constructors 

    // class AssignmentStatement : public Statement {
    //     public:
    //     AssignmentStatement(Token nodeToken, Token Variable, std::unique_ptr<Expression> Value) 
    //         : Statement(nodeToken), variable(Variable), value(std::move(Value)) {};

    //     Token variable;
    //     std::unique_ptr<Expression> value;
    // };


    // class VariableDefenitionStatement : public Statement {
    //     public:
    //     VariableDefenitionStatement(Token nodeToken, Token Type, Token Name) 
    //         : Statement(nodeToken), type(Type), name(Name) {};

    //     Token type;
    //     Token name;
    // };


    // class IfStatement : public Statement {
    //     public:
    //     IfStatement(Token nodeToken) : Statement(nodeToken) {};

    //     std::unique_ptr<Expression> condition;
    //     std::unique_ptr<std::vector<Statement>> ifbody;
    // };


    // class IfElseStatement : public Statement {
    //     public:
    //     IfElseStatement(Token nodeToken) : Statement(nodeToken) {};

    //     std::unique_ptr<Expression> condition;
    //     std::unique_ptr<std::vector<Statement>> ifbody;
    //     std::unique_ptr<std::vector<Statement>> elsebody;
    // };


    // class WhileStatement : public Statement {
    //     public:
    //     WhileStatement(Token nodeToken) : Statement(nodeToken) {};

    //     std::unique_ptr<Expression> condition;
    //     std::unique_ptr<std::vector<Statement>> ifbody;
    // };









// < ================ OTHER ================ >

class FunctionDefinition : public Node {
    public:
    FunctionDefinition(
                        Token Name, 
                        Token ReturnType, 
                        std::vector<std::pair<Token, Token>> Arguments, 
                        std::vector<std::unique_ptr<Statement>> Body
                      ) 
        : name(Name),
          returnType(ReturnType),
          arguments(Arguments),
          body(std::move(Body)) {};

    Token name;
    Token returnType;
    std::vector<std::pair<Token, Token>> arguments; // pairs <type, name>
    std::vector<std::unique_ptr<Statement>> body;

};




class Programm : public Node {
    public:
    Programm(std::vector<std::unique_ptr<FunctionDefinition>> Functions) 
        : functions(std::move(Functions)) {}

    // other stuff
    std::vector<std::unique_ptr<FunctionDefinition>> functions;
};




























// class BinaryOperation : public Expression {
// public:
//     std::std::unique_ptr<Expression> left;
//     std::std::unique_ptr<Expression> right;
    
//     // BinaryOperation(std::std::unique_ptr<Expression> l, std::std::unique_ptr<Expression> r, std::string o)
//     //     : left(std::move(l)), right(std::move(r)), op(std::move(o)) {}
    
//     void accept(Visitor& visitor) override;
// };










// // Интерфейс посетителя (Visitor pattern)
// class Visitor {
// public:
//     virtual ~Visitor() = default;
//     virtual void visit(IntegerLiteral& node) = 0;
//     virtual void visit(BinaryOperation& node) = 0;
//     virtual void visit(VariableDeclaration& node) = 0;
//     virtual void visit(BlockStatement& node) = 0;
//     // ... другие методы visit для других типов узлов
// };

// // Реализация accept для конкретных узлов
// void IntegerLiteral::accept(Visitor& visitor) { visitor.visit(*this); }
// void BinaryOperation::accept(Visitor& visitor) { visitor.visit(*this); }
// void VariableDeclaration::accept(Visitor& visitor) { visitor.visit(*this); }
// void BlockStatement::accept(Visitor& visitor) { visitor.visit(*this); }


