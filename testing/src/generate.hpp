#pragma once

#include "./parser.hpp"

class Generate{
public:
    Generate(NodeRoot&nodeRoot)
        : theRoot(std::move(nodeRoot))
    {}

    std::string generateAssembly(){
        std::stringstream strm;
        strm << "global _start" << "\n"
        << "_start:" << "\n"
        << "    mov rax, 60\n" 
        << "    mov rdi, " << theRoot.expr.intVal.value.value() << "\n"
        << "    syscall";
        return strm.str();
    }

private:
    NodeRoot theRoot;
};