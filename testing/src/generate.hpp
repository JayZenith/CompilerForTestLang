#pragma once


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
                gen->strm << "    push rax\n"; 
            }

            void operator()(const NodeExprIdent& expr_ident){
                
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
            void operator()(const NodeStmtLeave& stmt_leave) const {
                gen->gen_expr(stmt_leave.expr); //calls function to further advance assembly
                gen->strm << "    mov rax, 60\n";
                gen->strm << "    pop rdi\n";
                gen->strm << "    syscall\n"; 

            }

            void operator()(const NodeStmtLet& stmt_let){
                
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
        /*
        strm << "    mov rax, 60\n";
        strm << "    mov rdi, 0\n";
        strm << "    syscall\n"; 
        */
        return strm.str();
    }

private:
    std::stringstream strm;
    const NodeRoot root;
};