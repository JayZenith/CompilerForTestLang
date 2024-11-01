#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>


void readFile(std::string& contents, const std::string& file){
    std::stringstream contents_stream;
    std::fstream input(file, std::ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
    input.close();
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

    std::cout << contents;

}