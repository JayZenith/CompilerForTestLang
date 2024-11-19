#pragma once

#include <variant>
#include "./tokenize.hpp"

struct NodeExprIntLit{
    Token int_lit;
};

struct NodeExprIDENTIFIER{
    Token IDENTIFIER;
};

struct NodeExpr{ //[expr] is either int_lit or IDENTIFIER
    //Token NUMBER;
    std::variant<NodeExprIntLit, NodeExprIDENTIFIER> var;
};

struct NodeStmtLEAVE{  
    NodeExpr expr;
};

struct NodeStmtLET{ 
    Token IDENTIFIER;
    NodeExpr expr;
};

struct NodeStmt{ 
    std::variant<NodeStmtLEAVE, NodeStmtLET> var;
};

struct NodeRoot{  //[program]
    //NodeExpr expr;
    std::vector<NodeStmt> stmts; //[statements]

};

class Parser{
public:

    Parser(std::vector<Token> &x)
        : TokenType(std::move(x))
        {}

    //Can be either expr or IDENTIFIER 
    //so NodeExpr is a variant that holds specifically either
    std::optional<NodeExpr> parse_expr(){
        if(peek().value().type == TokenType::NUMBER){
            return NodeExpr{ .var = NodeExprIntLit { .int_lit = eat() } };
        }
        else if(peek().value().type == TokenType::IDENTIFIER){
            return NodeExpr{ .var = NodeExprIDENTIFIER { .IDENTIFIER = eat() } };
        }
        else {
            return {};
        }
    }
    
    //NodeStmt is a variant of the instruction types 
    //
    std::optional<NodeStmt> parse_stmt(){
        //check if a LEAVE(expr);
        if(peek().value().type == TokenType::LEAVE && peek(1).has_value() 
            && peek(1).value().type == TokenType::LEFT_PAREN){
            eat();
            eat();
            NodeStmtLEAVE stmt_LEAVE;
            /*
            struct NodeStmtLEAVE{  
                NodeExpr expr;
            };
            */
            if(auto node_expr = parse_expr()){
                stmt_LEAVE = {.expr = node_expr.value()};
                //std::cout << "hi";
            }
            
            //should eat to move index to next peek 
            eat();
            if(peek().has_value() && peek().value().type == TokenType::RIGHT_PAREN){ 
                eat();
            }
            if(peek().has_value() && peek().value().type == TokenType::SEMICOLON){
                eat();
            }
            return NodeStmt { .var = stmt_LEAVE };

        } else if (peek().has_value() && peek().value().type == TokenType::LET &&
            peek(1).has_value() && peek(1).value().type == TokenType::IDENTIFIER && 
            peek(2).has_value() && peek(2).value().type == TokenType::EQUAL){
            eat(); //consume LET
            auto stmt_LET = NodeStmtLET { .IDENTIFIER = eat() }; //consume IDENTIFIER 
            eat(); //consume EQUAL 
            /*
            struct NodeStmtLET{ 
                Token IDENTIFIER;
                NodeExpr expr;
            };
            I stored IDENTIFIERifier but now parsing for NUMBER 
            */
            if (auto expr = parse_expr()){
                stmt_LET.expr = expr.value();
                eat(); //dont we eat to look for SEMICOLON?
            } else { //not an int val or IDENTIFIER?
                std::cerr << "Invalid expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peek().has_value() && peek().value().type == TokenType::SEMICOLON){
                //std::cerr << "Expected ;" << std::endl;
                //exit(EXIT_FAILURE);
                eat();
            }
            return NodeStmt { .var = stmt_LET };
        } else{ 
            return {};
        }
        
    }

 
    std::optional<NodeRoot> parse_prog(){ 
        NodeRoot prog;
        while (peek().has_value()){ 
           
            if(auto stmt = parse_stmt()){
                prog.stmts.push_back(stmt.value());

            } else {
                std::cerr << "Invalid statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }

private:
    std::vector<Token> TokenType;
    std::optional<Token> peek(int offset=0){
        if(idx + offset > TokenType.size() - 1){
            return {};
        } else { 
            return TokenType.at(idx+offset);
        }
    }

    Token eat(){
        return TokenType.at(idx++);
    }

    void error(int line, std::string message){
        report(line, "", message);
    }

    void report(int line, std::string where, std::string message){
        std::cerr << "[line " << line << "] Error" << where << ": "
        << message << std::endl;
        hadError = true;
    }

    size_t idx = 0;
    bool hadError = false;
};