#pragma once


#include <memory>
#include "token.hpp"
#include "logger.hpp"
#include "../semanticsAnalyzer/IVisitor.hpp"







// Base class for every node in ast
class Node {
    public:
        Node() = delete; 
        virtual ~Node() = default;
        
        // virtual method for Visitor pattern
        virtual void accept(IVisitor* visitor) = 0;
        // add this function to every final class
        // void accept(IVisitor* visitor) override { visitor->visit(*this); } 

        
        // Prevent slicing
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        
        // Allow moving
        Node(Node&&) = default;
        Node& operator=(Node&&) = default;

        Token token;
    
    protected:
        Node(Token tok) : token(tok) {} 

};














// < ================ EXPRESSIONS ================ >


class Expression : public Node {
    public:
        virtual ~Expression() override = default;

    protected:
        Expression(Token tok) : Node(tok) {}
};


    class UnaryOperation final : public Expression {
        public:
        explicit UnaryOperation(
                                Token tok, 
                                std::unique_ptr<Expression> Value, 
                                TokenType Operation
                               ) 
            : Expression(tok), 
              value(std::move(Value)), 
              operation(Operation) {};


        std::unique_ptr<Expression> value;
        TokenType operation;

        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };


    class BinaryOperation final : public Expression {
        public:
        explicit BinaryOperation(
                                 Token tok, 
                                 std::unique_ptr<Expression> LeftValue, 
                                 std::unique_ptr<Expression> RightValue, 
                                 TokenType Operation
                                ) 
            : Expression(tok), 
              leftValue(std::move(LeftValue)), 
              rightValue(std::move(RightValue)), 
              operation(Operation) {};

        std::unique_ptr<Expression> leftValue;
        std::unique_ptr<Expression> rightValue;
        TokenType operation;

        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };


    class FunctionCall final : public Expression {
        public:
        explicit FunctionCall(
                              Token tok, 
                              std::unique_ptr<Expression> FunctionAdress, 
                              std::vector<std::unique_ptr<Expression>> Arguments
                             ) 
            : Expression(tok), 
              functionAdress(std::move(FunctionAdress)), 
              arguments(std::move(Arguments)) {};

        std::unique_ptr<Expression> functionAdress;
        std::vector<std::unique_ptr<Expression>> arguments;

        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };






// < ================ LITERALS and VARIABLES ================ >


class Primitive : public Expression {
    public:
        ~Primitive() override = default;
    
    protected:
        Primitive(Token tok) : Expression(tok) {};
};


    class IntegerLiteral final : public Primitive {
        public:
        explicit IntegerLiteral(Token literal) 
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

        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };


    class StringLiteral final : public Primitive {
        public:
        explicit StringLiteral(Token literal) 
            : Primitive(literal), value(literal.lexeme) {};
        
        std::string_view value;

        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };


    class CharLiteral final : public Primitive {
        public:
        explicit CharLiteral(Token literal) 
            : Primitive(literal), value(literal.lexeme.at(0)) {};
        
        char value;

        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };
        


    class Variable final : public Primitive {
        public:
        explicit Variable(Token tok) 
            : Primitive(tok), identifier(tok.lexeme) {};

        std::string_view identifier;

        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };











// < ================ STATEMENTS ================ >

class Statement : public Node {
    public:    
    ~Statement() override = default;
    
    protected:
    Statement(Token tok) : Node(tok) {}
};


    class ExpressionStatement final : public Statement {
        public:
        explicit ExpressionStatement(Token tok, std::unique_ptr<Expression> expr) 
            : Statement(tok), expression(std::move(expr)) {};

        std::unique_ptr<Expression> expression;

        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };


    class VariableDeclarationStatement final : public Statement {
        public:
        explicit VariableDeclarationStatement(Token tok, Token Type, Token Name) 
            : Statement(tok), type(Type), name(Name) {};

        Token type;
        Token name;
    
        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };


    class IfStatement final : public Statement {
        public:
        explicit IfStatement(
                        Token tok, 
                        std::unique_ptr<Expression> Condition, 
                        std::vector<std::unique_ptr<Statement>>&& IfBody, 
                        std::vector<std::unique_ptr<Statement>>&& ElseBody
                       ) 
            : Statement(tok), 
              condition(std::move(Condition)), 
              ifBody(std::move(IfBody)), 
              elseBody(std::move(ElseBody)) {};

        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> ifBody;
        std::vector<std::unique_ptr<Statement>> elseBody;
    
        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };


    class WhileStatement final : public Statement {
        public:
        explicit WhileStatement(
                                Token tok, 
                                std::unique_ptr<Expression> Condition, 
                                std::vector<std::unique_ptr<Statement>>&& Body) 
            : Statement(tok), 
              condition(std::move(Condition)), 
              body(std::move(Body)) {};

        std::unique_ptr<Expression> condition;
        std::vector<std::unique_ptr<Statement>> body;
    
        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };


    class ReturnStatement final : public Statement {
        public:
        explicit ReturnStatement(Token tok, std::unique_ptr<Expression> expr) 
            : Statement(tok), expression(std::move(expr)) {};

        std::unique_ptr<Expression> expression;
    
        void accept(IVisitor* visitor) override { visitor->visit(*this); }
    };










// < ================ OTHER ================ >

class FunctionDefinition final : public Node {
    public:
    explicit FunctionDefinition(
                        Token tok, 
                        Token Name, 
                        Token ReturnType, 
                        std::vector<std::pair<Token, Token>>&& Arguments,
                        std::vector<std::unique_ptr<Statement>>&& Body
                      ) 
        : Node(tok),
          name(Name), 
          returnType(ReturnType), 
          arguments(std::move(Arguments)), 
          body(std::move(Body)) {};

    Token name;
    Token returnType;
    std::vector<std::pair<Token, Token>> arguments; // pairs <type, name>
    std::vector<std::unique_ptr<Statement>> body;

    void accept(IVisitor* visitor) override { visitor->visit(*this); }
};




class Programm final : public Node {
    public:
    explicit Programm(
             std::vector<std::unique_ptr<FunctionDefinition>>&& Functions, 
             std::vector<std::unique_ptr<VariableDeclarationStatement>>&& Globals
            ) 
        : Node(Token(Position(0,0,0), TOKEN_END, "PROGRAMM ROOT")),
          functions(std::move(Functions)), 
          globalVariables(std::move(Globals)) {}

    // other stuff (metadata)
    std::vector<std::unique_ptr<FunctionDefinition>> functions;
    std::vector<std::unique_ptr<VariableDeclarationStatement>> globalVariables;

    void accept(IVisitor* visitor) override { visitor->visit(*this); }
};






