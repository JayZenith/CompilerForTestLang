#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>

#include "./tokenize.hpp"
#include "./parser.hpp"
#include "./generate.hpp"



void readFile(std::string&contents, std::string arg){
    std::fstream input(arg, std::ios::in);
    std::stringstream strm;
    strm << input.rdbuf();
    contents = strm.str();
}

int main(int argc, char* argv[]){
    /*
    if (argc < 2){
        std::cout << "here" <<  argv[1] << std::endl;
        std::cerr << "Incorrect usage. Correct use is: " << std::endl;
        std::cerr << "test <../input.test>" << std::endl;
        return EXIT_FAILURE;
    }
    */

   std::string contents;
   readFile(contents, "../input.test");
   Tokenize tokenizer(contents);
   std::vector<Token>TokenType = tokenizer.tokenize();
   std::cout << "hi";
   Parser parser(TokenType);
   std::optional<NodeRoot> prog = parser.parse_prog();
   //std::cout << "hi";
   //std::cout << expr.value().value().int_lit.value();
   std::cout << "hi";

   
   Generator generator(prog.value());

   
   std::fstream out("test.asm", std::ios::out);
   out << generator.generate_prog();

    



  
   

    return EXIT_SUCCESS;
}

