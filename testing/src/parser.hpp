#pragma once

#include <variant>
#include "./tokenize.hpp"
#include "./arena.hpp"

struct NodeExprIntLit{
    Token int_lit;
};

struct NodeExprStringLit{
    Token string_lit;
};

struct NodeExprIdent{
    Token ident;
};

struct NodeExpr;

//without the pointers, we would have to know the exact size of the 
//data types which is not possible, thus the pointers bypasses this as 
//we know theyre pointers to structs 

struct BinExprAdd{
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct BinExprMulti{    
    NodeExpr* lhs;
    NodeExpr* rhs;
};

struct NodeBinExpr{
    std::variant<BinExprAdd*, BinExprMulti*> var;
};

struct NodeExpr{ //[expr] is either int_lit or IDENTIFIER
    std::variant<NodeExprIntLit*, NodeExprIdent*, NodeExprStringLit*, NodeBinExpr*> var;
};


struct NodeStmtLeave{  
    NodeExpr* expr;
};

struct NodeStmtLet{ 
    Token ident;
    NodeExpr* expr;
};

struct NodeStmt{  
    std::variant<NodeStmtLeave*, NodeStmtLet*> var;
};

struct NodeRoot{  //[program]
    std::vector<NodeStmt*> stmts; //[statements]

};

class Parser{
public:
    Parser(std::vector<Token> &x)
        : TokenType(std::move(x)),
        allocator(1024 * 1024 * 4) //the arena 
        {}

    //obtaining the terminals 
    std::optional<NodeExpr*> parse_expr(){ 
        if(peek().value().type == TokenType::NUMBER){
            auto expr_int_lit = allocator.alloc<NodeExprIntLit>(); //allocate within the arena and get a pointer to the type
            expr_int_lit->int_lit = eat(); //must -> since its a pointer to the type 
            auto expr = allocator.alloc<NodeExpr>();
            expr->var = expr_int_lit; 
            return expr;
            //return NodeExpr { .var = node_expr_int_lit };
            //return NodeExpr{ .var = NodeExprIntLit { .int_lit = eat() } }; //eating token
        }
        else if(peek().value().type == TokenType::STRING){
            auto expr_string_lit = allocator.alloc<NodeExprStringLit>();
            expr_string_lit->string_lit = eat();
            auto expr = allocator.alloc<NodeExpr>();
            expr->var = expr_string_lit;
            return expr;
            //return NodeExpr{ .var = NodeExprStringLit { .string_lit = eat() } }; //eating token
        }
        else if(peek().value().type == TokenType::IDENTIFIER){
            auto expr_ident = allocator.alloc<NodeExprIdent>();
            expr_ident->ident = eat();
            auto expr = allocator.alloc<NodeExpr>();
            expr->var = expr_ident;
            return expr;
            //return NodeExpr{ .var = NodeExprIdent { .ident = eat() } };
        }
        else {
            return {};
        }
    }
    
    //NodeStmt is a variant of the instruction types 
    std::optional<NodeStmt*> parse_stmt(){
        if(peek().value().type == TokenType::LEAVE && peek(1).has_value() 
            && peek(1).value().type == TokenType::LEFT_PAREN){
            eat();
            eat();
            //NodeStmtLeave stmt_leave; 
            auto stmt_leave = allocator.alloc<NodeStmtLeave>();

            if(auto node_expr = parse_expr()){
                //stmt_leave = {.expr = node_expr.value()}; //will eat() as well
                stmt_leave->expr = node_expr.value();
            }
            
            if(peek().has_value() && peek().value().type == TokenType::RIGHT_PAREN){ 
                eat();
            }
            if(peek().has_value() && peek().value().type == TokenType::SEMICOLON){
                eat();
            }
            auto stmt = allocator.alloc<NodeStmt>();
            stmt->var = stmt_leave;
            return stmt;
            //return NodeStmt { .var = stmt_leave };

        } else if (peek().has_value() && peek().value().type == TokenType::LET &&
            peek(1).has_value() && peek(1).value().type == TokenType::IDENTIFIER && 
            peek(2).has_value() && peek(2).value().type == TokenType::EQUAL){
            eat(); //consume LET

            auto stmt_let = allocator.alloc<NodeStmtLet>();
            stmt_let->ident = eat();
            //auto stmt_let = NodeStmtLet { .ident = eat() }; //consume IDENTIFIER 
            
            eat(); //consume EQUAL 
            
            if (auto expr = parse_expr()){
                stmt_let->expr = expr.value();
                //stmt_let.expr = expr.value(); //had to .expr because has two data members in struct 
                //eat(); //dont we eat to look for SEMICOLON? no WE ATE DURING PARSING
            } else { //not an int val or IDENTIFIER?
                std::cerr << "Invalid expression" << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peek().has_value() && peek().value().type == TokenType::SEMICOLON){
                eat();
            }
            auto stmt = allocator.alloc<NodeStmt>();
            stmt->var = stmt_let;
            return stmt;
            //return NodeStmt { .var = stmt_let };
        } else{ 
            return {};
        }
        
    }

 
    std::optional<NodeRoot> parse_prog(){ 
        NodeRoot prog; //vector of stmts which holds variants
        while (peek().has_value()){ 
            if(auto stmt = parse_stmt()){
                prog.stmts.push_back(stmt.value());
                //std::cout << "hi";
            } else { //does not match current instructions supported 
                std::cerr << "Invalid statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }

private:
    std::vector<Token> TokenType;
    std::optional<Token> peek(int offset=0){
        if(idx + offset > TokenType.size() - 1){
            return {};
        } else { 
            return TokenType.at(idx+offset);
        }
    }

    Token eat(){ return TokenType.at(idx++); }

    void error(int line, std::string message){
        report(line, "", message);
    }

    void report(int line, std::string where, std::string message){
        std::cerr << "[line " << line << "] Error" << where << ": "
        << message << std::endl;
        hadError = true;
    }

    size_t idx = 0;
    bool hadError = false;
    ArenaAllocator allocator;
};