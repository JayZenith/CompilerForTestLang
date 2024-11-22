#pragma once

#include<unordered_map>
#include <cassert>

class Generator{

public:
    Generator(NodeRoot x)
        : root(std::move(x))
        {}

    void gen_expr(const NodeExpr* expr) {
        struct ExprVisitor{
            Generator* gen;
            
            void operator()(const NodeExprIntLit* expr_int_lit){
                gen->strm << "    mov rax, " << expr_int_lit->int_lit.value.value() << "\n";
                gen->push("rax");
            }

            void operator()(const NodeExprStringLit* expr_string_lit){
                //auto test = expr_string_lit.string_lit.value.value();
                //std::cout << "out";
                gen->strm << "section .data\n    theString db " << "'" <<expr_string_lit->string_lit.value.value() << "', 0Ah" << "\n";
                gen->strm << "section .text\n    global _start" << "\n" << "_start:\n";
                gen->strm << "    mov eax, 4\n    mov ebx, 1\n    lea ecx, [theString]\n    mov edx, 13\n";
                gen->strm << "    int 0x80\n";
            }

            void operator()(const NodeExprIdent* expr_ident){
                //std::cout << "of";
                const auto& var = gen->varsMap.at(expr_ident->ident.value.value());
               
                std::stringstream offset {};
                offset << "QWORD [rsp + " << (gen->stackSize - var.stackLoc - 1) * 8 << "]\n";
                gen->push(offset.str());
            }

            void operator()(const NodeBinExpr* bin_expr) const {
                assert(false); //not implemented 
            }
        };

        //deconstruct and set gen to this to then access the shit 
        ExprVisitor visitor {.gen = this}; 
        std::visit(visitor, expr->var);
    }

    void gen_stmt(const NodeStmt* stmt) {
        struct StmtVisitor{
            Generator* gen;
            //will call operator based on correct parameter 
            void operator()(const NodeStmtLeave* stmt_leave) const {
                gen->gen_expr(stmt_leave->expr); //calls function to further advance assembly
                gen->strm << "    mov rax, 60\n";
                gen->pop("rdi");
                //gen->strm << "    pop rdi\n";
                gen->strm << "    syscall\n"; 
            }

            void operator()(const NodeStmtLet* stmt_let){
                //put in map
                gen->varsMap.insert({stmt_let->ident.value.value(), Var{.stackLoc = gen->stackSize} });
                //put in stack
                for (auto i : gen->varsMap)
                std::cout << "\nyo: " << i.first << "\n" << std::endl;
                
                gen->gen_expr(stmt_let->expr);
               
            }
            void operator()(const NodeBinExpr* bin_expr) const {
                assert(false); //not implemented 
            }
        };
        StmtVisitor visitor { .gen = this }; //deconstruct and gen is a pointer member as is "this"
        std::visit(visitor, stmt->var); //calls operator
    }
    

    std::string generate_prog(){
        strm << "global _start\n_start:\n";
        //loop through the statements 
        for(const NodeStmt* stmt : root.stmts){
            gen_stmt(stmt);
        }
        //exit syscall return 0 as last resort 
        strm << "    mov rax, 60\n";
        strm << "    mov rdi, 0\n";
        strm << "    syscall\n"; 
        return strm.str();
    }

private:
    void push(const std::string& reg){
        strm << "    push " << reg << "\n";
        stackSize++;
    }

    void pop(const std::string& reg){
        strm << "    pop " << reg << "\n";
        stackSize--;
    }

    struct Var{
        //will later include type to make it statically typed
        size_t stackLoc; //to check pos in stack

    };

    std::stringstream strm;
    const NodeRoot root;
    size_t stackSize = 0;
    std::unordered_map<std::string, Var> varsMap {};

    
};