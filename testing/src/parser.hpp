#pragma once


#include "./tokenize.hpp"


struct NodeExpr{
    TokensStruct intVal;
};

struct NodeRoot{
    NodeExpr expr;
};

class Parser{

public:
    Parser(std::vector<TokensStruct> &x)
        : tokens(std::move(x))
        {}

    NodeRoot parse(){
        NodeRoot nodeRoot;

        while(peek().has_value()){
            //leave(expr);
            if(peek().value().type == Tokens::leave && peek(1).has_value() 
                && peek(1).value().type == Tokens::lp){
                eat();
                eat();
                
                if(peek().has_value() && peek().value().type == Tokens::intVal){ 
                    nodeRoot.expr = NodeExpr{ peek().value() };
                    eat();
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
        return nodeRoot;
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