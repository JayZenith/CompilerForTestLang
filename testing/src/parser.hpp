#pragma once

#include <variant>
#include "./tokenize.hpp"

struct NodeExprIntLit{
    TokensStruct int_lit;
};

struct NodeExprIdent{
    TokensStruct ident;
};

struct NodeExpr{ //[expr] is either int_lit or ident
    //TokensStruct intVal;
    std::variant<NodeExprIntLit, NodeExprIdent> var;
};

struct NodeStmtLeave{  
    NodeExpr expr;
};

struct NodeStmtLet{ 
    TokensStruct ident;
    NodeExpr expr;
};

struct NodeStmt{ 
    std::variant<NodeStmtLeave, NodeStmtLet> var;
};

struct NodeRoot{  //[program]
    //NodeExpr expr;
    std::vector<NodeStmt> stmts; //[statements]

};

class Parser{
public:

    Parser(std::vector<TokensStruct> &x)
        : tokens(std::move(x))
        {}

    //Can be either expr or ident 
    //so NodeExpr is a variant that holds specifically either
    std::optional<NodeExpr> parse_expr(){
        if(peek().value().type == Tokens::intVal){
            return NodeExpr{ .var = NodeExprIntLit { .int_lit = eat() } };
        }
        else if(peek().value().type == Tokens::ident){
            return NodeExpr{ .var = NodeExprIdent { .ident = eat() } };
        }
        else {
            return {};
        }
    }
    
    //NodeStmt is a variant of the instruction types 
    //
    std::optional<NodeStmt> parse_stmt(){
        //check if a leave(expr);
        if(peek().value().type == Tokens::leave && peek(1).has_value() 
            && peek(1).value().type == Tokens::lp){
            eat();
            eat();
            NodeStmtLeave stmt_leave;
            /*
            struct NodeStmtLeave{  
                NodeExpr expr;
            };
            */
            if(auto node_expr = parse_expr()){
                stmt_leave = {.expr = node_expr.value()};
                //std::cout << "hi";
            }
            
            //should eat to move index to next peek 
            eat();
            if(peek().has_value() && peek().value().type == Tokens::rp){ 
                eat();
            }
            if(peek().has_value() && peek().value().type == Tokens::semi){
                eat();
            }
            return NodeStmt { .var = stmt_leave };

        } else if (peek().has_value() && peek().value().type == Tokens::let &&
            peek(1).has_value() && peek(1).value().type == Tokens::ident && 
            peek(2).has_value() && peek(2).value().type == Tokens::eq){
            eat(); //consume let
            auto stmt_let = NodeStmtLet { .ident = eat() }; //consume ident 
            eat(); //consume eq 
            /*
            struct NodeStmtLet{ 
                TokensStruct ident;
                NodeExpr expr;
            };
            I stored identifier but now parsing for intVal 
            */
            if (auto expr = parse_expr()){
                stmt_let.expr = expr.value();
                eat(); //dont we eat to look for semi?
            } else { //not an int val or ident?
                std::cerr << "Invalid expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peek().has_value() && peek().value().type == Tokens::semi){
                //std::cerr << "Expected ;" << std::endl;
                //exit(EXIT_FAILURE);
                eat();
            }
            return NodeStmt { .var = stmt_let };
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
    std::vector<TokensStruct> tokens;
    std::optional<TokensStruct> peek(int offset=0){
        if(idx + offset > tokens.size() - 1){
            return {};
        } else { 
            return tokens.at(idx+offset);
        }
    }

    TokensStruct eat(){
        return tokens.at(idx++);
    }

    size_t idx = 0;
};