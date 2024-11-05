#pragma once


#include "./parser.hpp"
#include <sstream>


class Generator{

public:
    Generator(NodeRoot root)
        : root(std::move(root))
        {}

    std::string generate() const{
        std::stringstream output ;
        output << "global _main\n_main:\n";
        output << "    mov rax, 60\n";
        output << "    mov rdi, " << root.expr.intVal.value.value() << "\n";
        output << "    syscall";
        return output.str();
    }

private:
    const NodeRoot root;


};