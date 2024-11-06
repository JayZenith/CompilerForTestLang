#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>

#include "./tokenize.hpp"
#include "./parser.hpp"
#include "./generate.hpp"

void readFile(std::string& contents, std::string arg){
    std::stringstream contentsStream;
    std::fstream input(arg, std::ios::in);
    contentsStream << input.rdbuf();
    contents = contentsStream.str();
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
   std::vector<TokensStruct> tokens = tokenizer.tokenize();
   Parser parser(tokens);
   NodeRoot root = parser.parse();
   std::cout << "here: " << root.expr.intVal.value.value() << std::endl;
   Generate generator(root);
   std::string assembly = generator.generateAssembly();

   std::cout << assembly << std::endl;


   std::fstream openFile("test.asm", std::ios::out);
   openFile << assembly;

   system("nasm -felf64 test.asm");
   //system("ld -o test test.o");

   

    return EXIT_SUCCESS;
}

