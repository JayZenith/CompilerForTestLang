#pragma once


class Generator{

public:
    Generator(std::optional<NodeExpr>&x)
        : root(std::move(x))
        {}

    std::string generate(){
        std::stringstream strm;
        strm << "global _start\n" << "_start:\n" 
             << "    mov rax, 60\n" << "    mov rdi, " 
             << root.value().intVal.value.value() << "\n"
             << "    syscall";
        return strm.str();
    }

private:
    std::optional<NodeExpr> root;


};