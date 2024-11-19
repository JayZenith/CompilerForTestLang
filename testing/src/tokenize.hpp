#pragma once

#include <optional>
#include <iostream>
#include <vector>
#include <string>

enum class TokenType{
    // Single-character TokenType.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,  SEMICOLON, 
    DOT, MINUS, PLUS, SLASH, STAR, COMMA,


    // One or two character TokenType.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    //literals.
    NUMBER, IDENTIFIER, STRING,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, LET, VAR, WHILE,
    LEAVE, 
    };

struct Token{
    TokenType type;

    std::optional<std::string> value {}; //holds literal
    int theLine; //hold lne where instrruction is 
};

class Tokenize{
public:
    Tokenize(std::string&contents)
        : theContents(std::move(contents))
        {}

    std::vector<Token>tokenize(){
        std::vector<Token> TokenType;
        std::string buf = "";

        while(peek().has_value()){ //stops at eof, in which returns null 
            
            if(isalpha(peek().value())){ 
                while(peek().has_value() && isalnum(peek().value())){ //will figure out the start of a statement later on
                    buf.push_back(eat()); //push onto buffer and increment 
                }
                if(buf == "leave"){
                    TokenType.push_back({ .type = TokenType::LEAVE, .theLine=line});
                    buf.clear();
                    continue;
                } else if(buf == "let"){
                    TokenType.push_back({ .type = TokenType::LET, .theLine=line});
                    buf.clear();
                    continue;
                } else { //.value will hold the idnetifier symbol 
                    TokenType.push_back({ .type = TokenType::IDENTIFIER, .value=buf, .theLine=line });
                    buf.clear();
                    continue;
                }
            }
            
            else if(isspace(peek().value())){ //detects newlines
                if(peek().value() == '\n'){
                    line++; 
                }
                eat();
                //TokenType.push_back({ .type = TokenType::RIGHT_PAREN});
                buf.clear();
                continue;
            }
            else if(peek().value() == '('){
                buf.push_back(eat());
                TokenType.push_back({ .type = TokenType::LEFT_PAREN, .theLine=line});
                buf.clear();
                continue;
            }
            else if(isdigit(peek().value())){
                while(isdigit(peek().value())){ //8, 24, 255
                    buf.push_back(eat());
                }
                TokenType.push_back({ .type = TokenType::NUMBER, .value=buf, .theLine=line});
                buf.clear();
                continue;
            }
            else if(peek().value() == ')'){
                buf.push_back(eat());
                TokenType.push_back({ .type = TokenType::RIGHT_PAREN, .theLine=line});
                buf.clear();
                continue;
            }
            else if(peek().value() == ';'){
                buf.push_back(eat());
                TokenType.push_back({ .type = TokenType::SEMICOLON, .theLine=line});
                buf.clear();
                continue;
            }
            else if(peek().value() == '='){
                buf.push_back(eat());
                match('=') ? TokenType.push_back({ .type = TokenType::EQUAL_EQUAL, .theLine=line})
                    : TokenType.push_back({ .type = TokenType::EQUAL, .theLine=line});
                buf.clear();

                continue;
            }
            else if(peek().value() == '!'){
                buf.push_back(eat()); //dont need to push back on buffer
                match('=') ? TokenType.push_back({ .type = TokenType::BANG_EQUAL, .theLine=line})
                    : TokenType.push_back({ .type = TokenType::BANG, .theLine=line});
                buf.clear();
                continue;
            }
            else if(peek().value() == '<'){
                buf.push_back(eat()); //dont need to push back on buffer
                match('=') ? TokenType.push_back({ .type = TokenType::LESS_EQUAL, .theLine=line})
                    : TokenType.push_back({ .type = TokenType::LESS, .theLine=line});
                buf.clear();
                continue;
            }
            else if(peek().value() == '>'){
                buf.push_back(eat()); //dont need to push back on buffer
                match('=') ? TokenType.push_back({ .type = TokenType::GREATER_EQUAL, .theLine=line})
                    : TokenType.push_back({ .type = TokenType::GREATER, .theLine=line});
                buf.clear();
                continue;
            }
        }
        idx = 0;
        return TokenType;
    }

private:
    std::string theContents = "";

    std::optional<char> peek(int offset=0){ //
        if(theContents[idx+offset]){
            return theContents[idx+offset];
        } else {
            return {};
        }
    }

    char eat(){
        return theContents[idx++];
    }

    bool match(char expected){
        //the previous eat() causes peek to already look at expected
        if(peek().has_value()){ //check for following character, but what if whitespace?
        //I think we are fine, because we ex out eof, but now specifically check for char
            if(peek().value() != expected){
                return false;
            }
        } else {
            return false;
        }

        eat(); //eat the expected char
        return true;
    }
    

    size_t idx = 0;
    int line = 1;
    

};