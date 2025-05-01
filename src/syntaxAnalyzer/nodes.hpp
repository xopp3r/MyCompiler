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
        
    










// < ================ EXPRESSIONS ================ >

class Expression : public Node {
    public:
    virtual ~Expression() override = default;

    protected:
    Expression() = default;
};


    class ExpressionPriority_0 final : public Expression {
        public:
        ExpressionPriority_0(Token Variable, std::unique_ptr<Expression> Value) 
            : variable(Variable), value(std::move(Value)) {};

        Token variable;
        std::unique_ptr<Expression> value;
    };












    // class AssignmentStatement final : public Expression {
    //     public:
    //     AssignmentStatement(Token Variable, std::unique_ptr<Expression> Value) 
    //         : variable(Variable), value(std::move(Value)) {};

    //     Token variable;
    //     std::unique_ptr<Expression> value;
    // };














// < ================ STATEMENTS ================ >

class Statement : public Node {
    public:    
    ~Statement() override = default;
    

    protected:
    Statement() = default;
};


    class ExpressionStatement final : public Statement {
        public:
        ExpressionStatement(std::unique_ptr<Expression> expr) 
            : expression(std::move(expr)) {};

        std::unique_ptr<Expression> expression;
    };


    class VariableDeclarationStatement final : public Statement {
        public:
        VariableDeclarationStatement(Token Type, Token Name) 
            : type(Type), name(Name) {};

        Token type;
        Token name;
    };


    class IfStatement final : public Statement {
        public:
        IfStatement(
                        std::unique_ptr<Expression> Condition, 
                        std::vector<std::unique_ptr<Statement>>&& IfBody, 
                        std::vector<std::unique_ptr<Statement>>&& ElseBody
                       ) 
            : condition(std::move(Condition)), ifBody(std::move(IfBody)), elseBody(std::move(ElseBody)) {};

        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> ifBody;
        std::vector<std::unique_ptr<Statement>> elseBody;
    };


    class WhileStatement final : public Statement {
        public:
        WhileStatement(std::unique_ptr<Expression> Condition, std::vector<std::unique_ptr<Statement>>&& Body) 
            : condition(std::move(Condition)), body(std::move(Body)) {};

        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> body;
    };








// < ================ OTHER ================ >

class FunctionDefinition final : public Node {
    public:
    FunctionDefinition(
                        Token Name, 
                        Token ReturnType, 
                        std::vector<std::pair<Token, Token>>&& Arguments, 
                        std::vector<std::unique_ptr<Statement>>&& Body
                      ) 
        : name(Name), returnType(ReturnType), arguments(std::move(Arguments)), body(std::move(Body)) {};

    Token name;
    Token returnType;
    std::vector<std::pair<Token, Token>> arguments; // pairs <type, name>
    std::vector<std::unique_ptr<Statement>> body;

};




class Programm final : public Node {
    public:
    Programm(
             std::vector<std::unique_ptr<FunctionDefinition>>&& Functions, 
             std::vector<std::unique_ptr<VariableDeclarationStatement>>&& Globals
            ) 
        : functions(std::move(Functions)), globalVariables(std::move(Globals)) {}

    // other stuff (metadata)
    std::vector<std::unique_ptr<FunctionDefinition>> functions;
    std::vector<std::unique_ptr<VariableDeclarationStatement>> globalVariables;
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


