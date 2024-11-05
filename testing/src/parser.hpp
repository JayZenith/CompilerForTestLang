#pragma once 

#include "./tokenize.hpp"

struct NodeExpr{
    TokensStruct intVal;
};


struct NodeRoot{ //The goal is to place the desired expression in root and return the root 
    NodeExpr expr;
};

class Parser{

public:
    Parser(std::vector<TokensStruct>&tokens)
        : tokens(std::move(tokens))
        {}

    NodeRoot parse(){
        NodeRoot node_root = {};
        while(peak().has_value()){
            if(peak().value().type == Tokens::leave){ ///should be first 
                eat();
                continue;
            }
            
            if(peak().value().type == Tokens::lp){ //(Expr) parantheses correct ?
                if(peak(2).value().type == Tokens::rp){ //Just out of curiosity
                    eat();
                    continue;

                } else {
                    std::cerr << "Parantheses dont match" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            if(peak().value().type == Tokens::intVal){
                node_root.expr = NodeExpr { peak().value() }; 
                eat();
                continue;
            }

            if(peak().value().type == Tokens::rp){ //still need to read this paranthese
                eat();
                continue;
            }

            if(peak().value().type == Tokens::semi){ //still need to read this paranthese
                eat();
                continue;
            }

        }

        idx = 0;
        return node_root;
    }
    

private:
     std::vector<TokensStruct> tokens;
     size_t idx = 0;

    std::optional<TokensStruct> peak(int ahead = 0){ //we peak to see if character exists 
        if(idx + ahead >= tokens.size()){
            return {};
        }
        else{
            return tokens.at(idx + ahead);
        }
    }

    TokensStruct eat(){ //eat the character (place in buffer), and incremenet index 
        return tokens[idx++]; 
    }

};