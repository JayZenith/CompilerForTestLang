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
    TokensStruct intVal;
    //std::variant<NodeExprIntLit, NodeExprIdent> var;
};


struct NodeStmtLeave{  //leave returns an expression
    NodeExpr leave;
};

struct NodeStmtLet{ //let returns an expression but can be an ident 
    TokensStruct ident;
    NodeExpr expr;
};

struct NodeStmt{ //statements are either leavepexpr] or let ident=[expr];
    std::variant<NodeStmtLeave, NodeStmtLet> var;
};

struct NodeRoot{  //[program]
    NodeExpr expr;
    //std::vector<NodeStmt> stmts; //[statements]

};

class Parser{

public:
    Parser(std::vector<TokensStruct> &x)
        : tokens(std::move(x))
        {}

    std::optional<NodeExpr> parse_expr(){
        if(peek().value().type == Tokens::intVal){
            return NodeExpr{ eat() };
        } else {
            return {};
        }
    }

    std::optional<NodeExpr> parse(){
        std::optional<NodeExpr> nodeExpr;

        while(peek().has_value()){
            //leave(expr);
            
            if(peek().value().type == Tokens::leave && peek(1).has_value() 
                && peek(1).value().type == Tokens::lp){
                eat();
                eat();
                
                //should now approach a int_val or ident 
                //if(peek().has_value() && peek().value().type == Tokens::intVal || peek().value().type == Tokens::ident){ 
                if(peek().has_value()){
                    nodeExpr = parse_expr();
               
                    
                    //eat();
                    /*
                    if(peek().value().type == Tokens::intVal){
                        auto test = NodeExpr { .var=NodeExprIntLit { .int_lit = eat() } };
                    } else {
                        auto test = NodeExpr { .var=NodeExprIdent { .ident = eat() } };
                    }
                    */
                   
                    
                    if(peek().has_value() && peek().value().type == Tokens::rp){ 
                        eat();
                        if(peek().has_value() && peek().value().type == Tokens::semi){
                            eat();
                            //continue;
                        } else { //leave()
                            std::cerr << "Missing semicolon" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    } else { //leave(expr;
                        std::cerr << "Missing closing parantheses" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                } else { //leave();
                    std::cerr << "Missing argument for leave" << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else { //leaveexpr);
                std::cerr << "Missing opening parantheses" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        idx = 0;
        return nodeExpr;
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