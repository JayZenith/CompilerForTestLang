#pragma once

#include <optional>
#include <string>
#include <iostream>
#include <vector>


enum class Tokens{ leave, lp, intVal, rp, semi };

struct TokensStruct{
    Tokens type;
    std::optional<std::string> value {};
};

class Tokenize{

public:
    Tokenize(const std::string contents)
        : theContents(std::move(contents))
    {}

    std::vector<TokensStruct> tokenize(){
        std::string buf = "";
        std::vector<TokensStruct> tokens;

        if(peak().has_value() && isalpha(peak().value())){
            buf.push_back(eat());
            while(peak().has_value()){
                if(isalpha(peak().value())){
                    buf.push_back(eat());
                    if(buf == "leave"){
                        tokens.push_back({ .type=Tokens::leave });
                        buf.clear();
                        continue;
                    }
                }
                else if(peak().value() == '('){
                    buf.push_back(eat());
                    tokens.push_back({ .type=Tokens::lp });
                    buf.clear();
                    continue;
                }
                else if(isdigit(peak().value())){
                    //buf.push_back(eat());
                    while(isdigit(peak().value())){
                        buf.push_back(eat());
                    }
                    tokens.push_back({ .type=Tokens::intVal, .value=buf });
                    buf.clear();
                    continue;
                }
                else if(peak().value() == ')'){
                    buf.push_back(eat());
                    tokens.push_back({ .type=Tokens::rp });
                    buf.clear();
                    continue;
                }
                else if(peak().value() == ';'){
                    buf.push_back(eat());
                    tokens.push_back({ .type=Tokens::semi });
                    buf.clear();
                    continue;
                }
            }
        } else {
            std::cerr << "Incorrect syntax" << std::endl;
            exit(EXIT_FAILURE);
        }
        //std::cout << "charli brat" << std::endl;
        idx = 0;
        return tokens;
    }


private:
    std::string theContents = "";
    size_t idx = 0;

    std::optional<char> peak(){ //we peak to see if character exists 
        if(theContents[idx]){
            return theContents[idx];
        }
        else{
            return {};
        }
    }

    char eat(){ //eat the character (place in buffer), and incremenet index 
        return theContents[idx++]; 
    }

};