#pragma once

#include<unordered_map>

class Generator{

public:
    Generator(NodeRoot x)
        : root(std::move(x))
        {}

    void gen_expr(const NodeExpr& expr) {
        struct ExprVisitor{
            Generator* gen;
            

            void operator()(const NodeExprIntLit& expr_int_lit){
                gen->strm << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                gen->push("rax");
                //gen->strm << "    push rax\n"; 
            }

            void operator()(const NodeExprIDENTIFIER& expr_IDENTIFIER){
                /*
                if(!(gen->varsMap.find(expr_IDENTIFIER.IDENTIFIER.value.value()) == gen->varsMap.end())){
                    std::cerr << "Undeclared IDENTIFIERifier: " << expr_IDENTIFIER.IDENTIFIER.value.value() << std::endl;
                    exit(EXIT_FAILURE);
                }
                */
                const auto& var = gen->varsMap.at(expr_IDENTIFIER.IDENTIFIER.value.value());
                std::stringstream offset {};
                offset << "QWORD [rsp + " << (gen->stackSize - var.stackLoc - 1) * 8 << "]\n";
                gen->push(offset.str());
                

            }
        };

        //deconstruct and set gen to this to then access the shit 
        ExprVisitor visitor {.gen = this}; 
        std::visit(visitor, expr.var);
    }

    void gen_stmt(const NodeStmt& stmt) {
        struct StmtVisitor{
            Generator* gen;
            //will call operator based on correct parameter 
            void operator()(const NodeStmtLEAVE& stmt_LEAVE) const {
                gen->gen_expr(stmt_LEAVE.expr); //calls function to further advance assembly
                gen->strm << "    mov rax, 60\n";
                gen->pop("rdi");
                //gen->strm << "    pop rdi\n";
                gen->strm << "    syscall\n"; 

            }

            void operator()(const NodeStmtLET& stmt_LET){
                
                /*
                if(gen->varsMap.contains(stmt_LET.IDENTIFIER.value.value())){
                    std::cerr << "IDENTIFIERifier already declared: " << stmt_LET.IDENTIFIER.value.value() << std::endl;
                    exit(EXIT_FAILURE);
                }
                */
                //put in map
                gen->varsMap.insert({stmt_LET.IDENTIFIER.value.value(), Var{.stackLoc = gen->stackSize} });
                //put in stack
                for (auto i : gen->varsMap)
                std::cout << "\nyo: " << i.first << "\n" << std::endl;
                
                gen->gen_expr(stmt_LET.expr);
               
            }
        };
        StmtVisitor visitor { .gen = this }; //deconstruct and gen is a pointer member as is "this"
        std::visit(visitor, stmt.var); //calls operator
    }
    

    std::string generate_prog(){
        //std::stringstream strm;
        strm << "global _start\n_start:\n";

        //loop through the statements 
        for(const NodeStmt& stmt : root.stmts){
            //std::cout << "blood";
            gen_stmt(stmt);
        }


        //exit syscall return 0
        
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