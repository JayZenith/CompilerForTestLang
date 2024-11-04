#pragma once

#include <optional>
#include <string>
#include <vector>

enum class Tokens{ leave, lp, intVal, rp };

struct TokensStruct{
    Tokens type;
    std::optional<Tokens> value {};

};


class Tokenizer{

public: 
    Tokenizer(const std::string& contents) 
        : contents(std::move(contents))
        {}

    std::vector<TokensStruct> tokenize(){
        std::vector<TokensStruct> tokens;
        std::string buf = "";
        

        while(peak().has_value()){
            if(isalpha(peak().value())){
                buf.push_back(peak().value());
                consume();
                std::cout << "Here" << std::endl;
            }
        }

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

    std::optional<char> consume(){
        if(idx > contents.length() - 1){  //leave(9); length: 9 but length: 8 with indexing. Thus, if idx > 8 stop.
            return {};
        } else {
            return contents[idx++]; //return character and increment index 
        }
    }

    size_t idx = 0;

};