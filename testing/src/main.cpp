#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>
#include <chrono>

#include "./tokenization.hpp"
#include "./parser.hpp"

void readFile(std::string& contents, const std::string& fileName){
    std::stringstream contents_stream;
    std::fstream input(fileName, std::ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
}


auto start = std::chrono::steady_clock::now();

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
    Tokenizer tokenize(contents);
    std::vector<TokensStruct> tokens = tokenize.tokenize();
    Parser parser(tokens);
    parser.parse();

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout <<  std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
    return EXIT_SUCCESS;
}

