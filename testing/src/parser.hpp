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
            if(peek().value().type == Tokens::leave){
                eat();
                continue;
            }
            else if(peek().value().type == Tokens::lp){
                eat();
                continue;
            }
            else if(peek().value().type == Tokens::intVal){
                nodeRoot.expr = NodeExpr{ peek().value() };
                eat();
                continue;
            }
            else if(peek().value().type == Tokens::rp){
                eat();
                continue;
            }
            else if(peek().value().type == Tokens::semi){
                eat();
                continue;
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