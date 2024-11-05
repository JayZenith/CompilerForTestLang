#pragma once

#include <iostream>
#include "./tokenization.hpp"

struct NodeExpr{
    TokensStruct intVal;
};

struct NodeRoot{
    NodeExpr expr;

};


class Parser{
public:
    Parser(std::vector<TokensStruct>tokens)
        : tokens(std::move(tokens))
        {}


    NodeRoot parse(){
        NodeRoot root;
        
        while(peak().has_value()){
            if(peak().value().type == Tokens::leave){
                consume();
                continue;
            }
            else if(peak().value().type == Tokens::lp){ //can peak ahead to see if rp there too and a semi?
                consume();
                continue;
            }
            else if(peak().value().type == Tokens::intVal){
                root.expr = NodeExpr{ peak().value() };
                consume();
                continue;
            }
            else if(peak().value().type == Tokens::rp){ //can peak ahead to see if rp there too and a semi?
                consume();
                continue;
            }
            else if(peak().value().type == Tokens::semi){ //can peak ahead to see if rp there too and a semi?
                consume();
                continue;
            }
            else{
                std::cerr << "Not part of the langauge syntax" << std::endl;
                exit(EXIT_FAILURE);
            }
            
        }
        std::cout << "Fire" << std::endl;

        idx = 0;
        return root;
    }

private:
    std::vector<TokensStruct> tokens;

    std::optional<TokensStruct> peak(int ahead=0){ //check to see if character exists to consume 
        if(idx + ahead > tokens.size() - 1){
            return {};
        }
        else { //option to peak beyond current index 
            return tokens.at(ahead + idx);
        } 
    }

    std::optional<TokensStruct> consume(){
        if(idx > tokens.size() - 1){  //leave(9); length: 9 but length: 8 with indexing. Thus, if idx > 8 stop.
            return {};
        } else {
            return tokens.at(idx++); //return character and increment index 
        }
    }
    size_t idx = 0;


};