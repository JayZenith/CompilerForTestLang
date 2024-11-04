#pragma once

#include <optional>
#include <string>
#include <vector>

enum class Tokens{ leave, lp, intVal, rp, semi };

struct TokensStruct{
    Tokens type;
    std::optional<std::string> value {};

};


class Tokenizer{

public: 
    Tokenizer(const std::string& contents) 
        : contents(std::move(contents))
        {}

    std::vector<TokensStruct> tokenize(){
        std::vector<TokensStruct> tokens;
        std::string buf = "";
        

        if(peak().has_value() && isalpha(peak().value())){ //Make sure first character is correct 
            buf.push_back(consume()); //PUSH VALUE AND INCREMENT COUNT
            //consume();
            while(peak().has_value()){ //for remainder of content  //can take letters and numbers 
            
                
                if(isalpha(peak().value())){
                    std::cout << peak().value() << std::endl;
                    buf.push_back(consume()); //PUSH VALUE AND INCREMENT COUNT
                    if(buf == "leave"){
                        tokens.push_back({.type=Tokens::leave});
                        buf.clear();
                        continue;
                        //std::cout << "Here" << std::endl;

                    }
                } //end if 
                else if(peak().value() == '('){
                    buf.push_back(consume()); //PUSH VALUE AND INCREMENT COUNT
                    tokens.push_back({.type=Tokens::lp});
                    buf.clear();
                    continue;
                }
                else if(isdigit(peak().value())){
                    std::cout << "made it" << std::endl;
                    while(isdigit(peak().value())){
                        buf.push_back(consume()); //PUSH VALUE AND INCREMENT COUNT
                    }
                    tokens.push_back({.type=Tokens::intVal, .value=buf});
                    buf.clear();
                    continue;
                }
                else if(peak().value() == ')'){
                    buf.push_back(consume()); //PUSH VALUE AND INCREMENT COUNT
                    tokens.push_back({.type=Tokens::rp});
                    buf.clear();
                    continue;
                }
                else if(peak().value() == ';'){
                    buf.push_back(consume()); //PUSH VALUE AND INCREMENT COUNT
                    tokens.push_back({.type=Tokens::semi});
                    buf.clear();
                    continue;
                }

            } //end while
            
                
        } else {
            std::cerr << "Not a leave statement" << std::endl;
            exit(EXIT_FAILURE);
        }

        idx = 0;
        std::cout << "Here" << std::endl;
        return tokens;
    }

private:
    const std::string contents = "";

    std::optional<char> peak(int ahead=0){ //check to see if character exists to consume 
        if(contents[ahead + idx]){ //option to peak beyond current index 
            return contents[ahead + idx];
        } else {
            return {};
        }
    }

    char consume(){
        if(idx > contents.length() - 1){  //leave(9); length: 9 but length: 8 with indexing. Thus, if idx > 8 stop.
            return 0;
        } else {
            return contents[idx++]; //return character and increment index 
        }
    }

    size_t idx = 0;

};