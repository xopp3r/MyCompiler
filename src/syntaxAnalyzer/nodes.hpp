#pragma once


#include <memory>
#include "../tokenizer/token.hpp"
#include "../common/logger.hpp"
#include "IVisitor.hpp"







// Base class for every node in ast
class Node {
    public:
        virtual ~Node() = default;
        
        // virtual method for Visitor pattern
        virtual void accept(IVisitor* visitor) const = 0;
        // void accept(IVisitor* visitor) { visitor->visit(*this); }  // add this function to every final class

        
        // Prevent slicing
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        
        // Allow moving
        Node(Node&&) = default;
        Node& operator=(Node&&) = default;
    
    protected:
        Node() = default; 
    };














// < ================ EXPRESSIONS ================ >


class Expression : public Node {
    public:
    virtual ~Expression() override = default;

    protected:
    Expression() = default;
};


    class UnaryOperation final : public Expression {
        public:
        UnaryOperation(std::unique_ptr<Expression> Value, TokenType Operation) 
            : value(std::move(Value)), operation(Operation) {};

        std::unique_ptr<Expression> value;
        TokenType operation;

        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };


    class BinaryOperation final : public Expression {
        public:
        BinaryOperation(std::unique_ptr<Expression> LeftValue, std::unique_ptr<Expression> RightValue, TokenType Operation) 
            : leftValue(std::move(LeftValue)), rightValue(std::move(RightValue)), operation(Operation) {};

        std::unique_ptr<Expression> leftValue;
        std::unique_ptr<Expression> rightValue;
        TokenType operation;

        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };


    class FunctionCall final : public Expression {
        public:
        FunctionCall(std::unique_ptr<Expression> FunctionAdress, std::vector<std::unique_ptr<Expression>> Arguments) 
            : functionAdress(std::move(FunctionAdress)), arguments(std::move(Arguments)) {};

        std::unique_ptr<Expression> functionAdress;
        std::vector<std::unique_ptr<Expression>> arguments;

        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };






// < ================ LITERALS ================ >


class Primitive : public Expression {
    public:
    Primitive(Token nodeToken) : token(nodeToken) {};
    
    ~Primitive() override = default;
    
    Token token;
};


    class IntegerLiteral final : public Primitive {
        public:
        IntegerLiteral(Token literal) 
            : Primitive(literal) {
            try {
                value = std::stoi(token.lexeme.data()); // string to int
            } catch (std::invalid_argument const& ex) {
                ERROR("Invalid number literal at " << token.position.string() << "\n");
            } catch (std::out_of_range const& ex) {
                ERROR("Number literal at " << token.position.string() << " is too big for int type\n");
            }
        };
        
        int value;

        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };


    class StringLiteral final : public Primitive {
        public:
        StringLiteral(Token literal) 
            : Primitive(literal), value(literal.lexeme) {};
        
        std::string value;

        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };


    class CharLiteral final : public Primitive {
        public:
        CharLiteral(Token literal) 
            : Primitive(literal), value(literal.lexeme.at(0)) {};
        
        char value;

        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };
        


    class Variable final : public Primitive {
        public:
        Variable(Token Identifier) 
            : Primitive(Identifier) {};

        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };











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

        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };


    class VariableDeclarationStatement final : public Statement {
        public:
        VariableDeclarationStatement(Token Type, Token Name) 
            : type(Type), name(Name) {};

        Token type;
        Token name;
    
        void accept(IVisitor* visitor) { visitor->visit(*this); }
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
    
        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };


    class WhileStatement final : public Statement {
        public:
        WhileStatement(std::unique_ptr<Expression> Condition, std::vector<std::unique_ptr<Statement>>&& Body) 
            : condition(std::move(Condition)), body(std::move(Body)) {};

        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> body;
    
        void accept(IVisitor* visitor) { visitor->visit(*this); }
    };


    class ReturnStatement final : public Statement {
        public:
        ReturnStatement(std::unique_ptr<Expression> expr) 
            : expression(std::move(expr)) {};

        std::unique_ptr<Expression> expression;
    
        void accept(IVisitor* visitor) { visitor->visit(*this); }
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

    void accept(IVisitor* visitor) { visitor->visit(*this); }
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

    void accept(IVisitor* visitor) { visitor->visit(*this); }
};






