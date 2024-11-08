#pragma once

#include <optional>
#include <iostream>
#include <vector>
#include <string>


enum class Tokens{ leave, lp, intVal, rp, semi, ident};

struct TokensStruct{
    Tokens type;
    std::optional<std::string> value {};

};

class Tokenize{
public:
    Tokenize(std::string&contents)
        : theContents(std::move(contents))
        {}

    std::vector<TokensStruct>tokenize(){
        std::vector<TokensStruct> tokens;
        std::string buf = "";

        while(peek().has_value()){ //leave(8);
            if(isalpha(peek().value())){ //leave 
                buf.push_back(eat());
                if(buf == "leave"){
                    tokens.push_back({ .type = Tokens::leave});
                    buf.clear();
                    continue;
                }
                /*
                else{
                    tokens.push_back({ .type = Tokens::ident, .value=buf});
                    buf.clear();
                    continue;
                }
                */
            }
            else if(peek().value() == '('){
                buf.push_back(eat());
                tokens.push_back({ .type = Tokens::lp});
                buf.clear();
                continue;
            }
            else if(isdigit(peek().value())){
                while(isdigit(peek().value())){ //8, 24, 255
                    buf.push_back(eat());
                }
                tokens.push_back({ .type = Tokens::intVal, .value=buf});
                buf.clear();
                continue;
            }
            else if(peek().value() == ')'){
                buf.push_back(eat());
                tokens.push_back({ .type = Tokens::rp});
                buf.clear();
                continue;
            }
            else if(peek().value() == ';'){
                buf.push_back(eat());
                tokens.push_back({ .type = Tokens::semi});
                buf.clear();
                continue;
            }
        }
    
        idx = 0;
        return tokens;
    }

private:
    std::string theContents = "";

    std::optional<char> peek(int offset=0){
        if(theContents[idx+offset]){
            return theContents[idx+offset];
        } else {
            return {};
        }
    }

    char eat(){
        return theContents[idx++];
    }

    size_t idx = 0;

};