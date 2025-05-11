#pragma once



#include "recursiveParser.hpp"






// checks if token is one of provided types
template <TokenType... variants>
constexpr bool isTokenOneOf(TokenType tokenType) {
    return ((tokenType == variants) or ...);
}



// does exacly what name says
template<int operationPriority, TokenType... AllowedTokensTypes> 
requires (operationPriority >= 3)
std::unique_ptr<Expression> parseLeftAssociativeBinaryOperationSequence(){
    
    auto left = parsePriority<operationPriority - 1>(); // parse higher priority expressions first
    
    // while priority of current operation match
    while (isTokenOneOf<AllowedTokensTypes...>(currentToken.type)) { 

        // save current operator
        Token op = consumeToken(currentToken.type);
        
        // parse next trem
        auto right = parsePriority<operationPriority - 1>();
        
        // build tree and left rotate
        left = std::make_unique<BinaryOperation>(
            op,
            std::move(left), 
            std::move(right), 
            op.type
        );
    }
    
    return left;
}




// same as left version, exept with right associativity
template <int OperationPriority, TokenType... AllowedTokensTypes>
std::unique_ptr<Expression> parseRightAssociativeBinaryOperationSequence() {

    // parse most left expr untill operator matches priority
    auto left = parsePriority<OperationPriority - 1>();

    if (isTokenOneOf<AllowedTokensTypes...>(currentToken.type)) {

        Token op = consumeToken(currentToken.type);
        
        // recursivly parse right expression for right-to-left associativity
        auto right = parseRightAssociativeBinaryOperationSequence<OperationPriority, AllowedTokensTypes...>();
        
        left = std::make_unique<BinaryOperation>(
            op,
            std::move(left),
            std::move(right),
            op.type
        );
    }
    
    return left;
}





// TODO: explain
template <int priority> requires (priority >= 0 and priority <= 8)
std::unique_ptr<Expression> parsePriority(void){


    if constexpr (priority == 0){

        // priority_0             
        // : <IDENTIFIER> | <STRING_LITERAL> | <NUMBER_LITERAL> | <CHAR_LITERAL> 
        // | '(' expression ')'
        // ;
        switch (currentToken.type) {
            case TOKEN_CHAR:
                return std::make_unique<CharLiteral>(consumeToken(TOKEN_CHAR));
        
            case TOKEN_STRING:
                return std::make_unique<StringLiteral>(consumeToken(TOKEN_STRING));
        
            case TOKEN_INTEGER:
                return std::make_unique<IntegerLiteral>(consumeToken(TOKEN_INTEGER));
            
            case TOKEN_IDENTIFIER:
                return std::make_unique<Variable>(consumeToken(TOKEN_IDENTIFIER));
        
            case TOKEN_PARENTHESES_OPEN: // parentheses enclosed expression (expr)
            {
                discardToken(TOKEN_PARENTHESES_OPEN);
                auto expr = parseExpression(); // from inside
                discardToken(TOKEN_PARENTHESES_CLOSE);
                return expr;
            };
            
            default:
                ERROR("parser error at token: '" << currentToken.lexeme << "' ("  << tokenName(currentToken.type) 
                        << ") at " << currentToken.position.string() << " : " << "Invalid expression primitive");
                break;
            }


    } else if constexpr (priority == 1){

        // priority_1
        //     : priority_0 (( '[' expression ']' ) |
        //                   ( '(' (expression (',' expression)*)? ')' ))*
        //     ;
        auto left = parsePriority<0>(); // parse higher priority expressions first
    
        // while priority of current operation match
        while (currentToken.type == TOKEN_SQUARE_BRACE_OPEN or 
               currentToken.type == TOKEN_PARENTHESES_OPEN) { 
    
            // save current token
            Token consumedToken = consumeToken(currentToken.type);
            
    
            // parse arguments
            if (consumedToken.type == TOKEN_SQUARE_BRACE_OPEN){
                // array indexing
                auto right = parseExpression();
                discardToken(TOKEN_SQUARE_BRACE_CLOSE);
    
                // converting arr[i] to @(arr + i)
                // some problems with tokens and positioning)) FIXME
                left = std::make_unique<BinaryOperation>(Token(consumedToken.position, TOKEN_OP_PLUS, "+"), std::move(left), std::move(right), TOKEN_OP_PLUS);
                left = std::make_unique<UnaryOperation>(Token(consumedToken.position, TOKEN_OP_DEREFERENCE, "@"), std::move(left), TOKEN_OP_DEREFERENCE);
    
            } else {
                // function call
                auto arguments = parseFunctionCallArguments();
                discardToken(TOKEN_PARENTHESES_CLOSE);
    
                left = std::make_unique<FunctionCall>(consumedToken, std::move(left), std::move(arguments));
            }
    
    
        }
        
        return left;


    } else if constexpr (priority == 2){
        
        // priority_2
        // : priority_1
	    // | ('&' | '@' | '-' | '!') priority_2
        // | '[' type ']' priority_2
        // ;
        std::unique_ptr<Expression> expr;

        if (isTokenOneOf<TOKEN_OP_MINUS, TOKEN_OP_NOT, TOKEN_OP_ADRESS, TOKEN_OP_DEREFERENCE>(currentToken.type)) {

            Token op = consumeToken(currentToken.type);
            
            // recursivly parse inner expression for right-to-left associativity
            auto innerExpr = parsePriority<2>();
            
            expr = std::make_unique<UnaryOperation>(op, std::move(innerExpr), op.type);

        } else if (currentToken.type == TOKEN_SQUARE_BRACE_OPEN) {

            discardToken(TOKEN_SQUARE_BRACE_OPEN);
            Token castType = consumeToken(TOKEN_KEYWORD_TYPE);
            discardToken(TOKEN_SQUARE_BRACE_CLOSE);
            
            // recursivly parse inner expression for right-to-left associativity
            auto innerExpr = parsePriority<2>();
            
            // TODO MB it's binary operator
            expr = std::make_unique<UnaryOperation>(castType, std::move(innerExpr), TOKEN_KEYWORD_TYPE);

        } else {
            expr = parsePriority<1>();
        }
        
        return expr;


    } else if constexpr (priority == 3){
        // priority_3
        // : priority_2 (('*' | '/' | '%') priority_2)* 
        // ;
        return parseLeftAssociativeBinaryOperationSequence
                <3, TOKEN_OP_MUL, TOKEN_OP_DIV, TOKEN_OP_MOD>();


    } else if constexpr (priority == 4){
        // priority_4
        // : priority_3 (('+' | '-') priority_3)*
        // ;
        return parseLeftAssociativeBinaryOperationSequence
                <4, TOKEN_OP_PLUS, TOKEN_OP_MINUS>();           
                
                
    } else if constexpr (priority == 5){
        // priority_5
        // : priority_4 (('>' | '<' | '>=' | '<=' | '==' | '!=') priority_4)*
        // ;
        return parseLeftAssociativeBinaryOperationSequence
                <5, TOKEN_OP_GREATER, TOKEN_OP_GREATER_EQ, TOKEN_OP_LESS, 
                    TOKEN_OP_LESS_EQ, TOKEN_OP_EQUAL, TOKEN_OP_NOT_EQUAL>();   
                    
                    
    } else if constexpr (priority == 6){
    
        // priority_6
        // : priority_5 ('&&' priority_5)*
        // ;
        return parseLeftAssociativeBinaryOperationSequence
                <6, TOKEN_OP_AND>();           
                
                
    } else if constexpr (priority == 7){
        
        // priority_7
        // : priority_6 ('||' priority_6)*
        // ;
        return parseLeftAssociativeBinaryOperationSequence
                <7, TOKEN_OP_OR>();   


    } else if constexpr (priority == 8){
        
        // priority_8
        // : priority_7 ('=' priority_8)? 
        // ;
        return parseRightAssociativeBinaryOperationSequence
                <8, TOKEN_OP_ASSIGNMENT>();
        

    } else static_assert(priority >= 0 && priority <= 8, 
        "Invalid priority level in parsePriority\nMain expression parsing function (parsePriority) compiling error"
      );


}


