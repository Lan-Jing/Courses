#include <stdarg.h>

#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <unordered_map>

// push a given number of operands
#define ops_pushback() \
{ \
            va_list ap; va_start(ap, _nops); \
            for(int i = 0;i < _nops;i++) \
                ops.push_back(va_arg(ap, node*)); \
            va_end(ap); \
} 

// statement type
typedef enum {
    BLOCK,
    LOCALVARDECL,
    ASSIGNSTMT,
    RETURNSTMT,
    IFSTMT,
    WRITESTMT,
    READSTMT
} stmttype;

// expression type
typedef enum {
    INUM,
    FNUM,
    ID,
    EXPR,
    FUNC
} exprtype;

/*
    base class for all nonterminals. 
    This is to provide a unified API to parse the syntax tree. See method print_tree_dfs()
*/
class node {
public:
    node() { Nops=0; }
    virtual ~node() { for(int i = 0;i < Nops;i++) delete ops[i]; }
    virtual string get_node_type() { return "Base"; }

    int Nops;
    vector<node*> ops;
    // # of operands and vector of operands
};

class Program: public node {
public:
    virtual string get_node_type() { return "Program"; }
    Program(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class Type: public node {
public:
    virtual string get_node_type() {
        switch (dtype) {
            case 0:  return "INT";
            case 1:  return "REAL";
            case 2:  return "STRING";
            default: return "Unknown Type"; 
        }
    }
    Type(int _dtype) { dtype=_dtype; }

    int dtype;
};

class MethodDecls: public node {
public:
    virtual string get_node_type() { return "MethodDecls"; }
    MethodDecls(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class MethodDecl: public node {
public:
    virtual string get_node_type() { return "MethodDecl"; }
    MethodDecl(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class FormalParams: public node {
public:
    virtual string get_node_type() { return "FormalParams"; }
    FormalParams(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class Block: public node {
public:
    virtual string get_node_type() { return "Block"; }
    Block(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class Statements: public node {
public:
    virtual string get_node_type() { return "Statements"; }
    Statements(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class Statement: public node {
public:
    virtual string get_node_type() {
        switch (stype) {
            case 0: return "BLOCK";
            case 1: return "LOCALVARDECL";
            case 2: return "ASSIGNSTMT";
            case 3: return "RETURNSTMT";
            case 4: return "IFSTMT";
            case 5: return "WRITESTMT";
            case 6: return "READSTMT";
        }
        return "Unknown";
    }
    Statement(stmttype _stype, int _nops, ...) { 
        stype=_stype; 
        Nops=_nops; ops_pushback(); 
    }

    stmttype stype;
};

class LocalVarDecl: public node {
public:
    virtual string get_node_type() { return "LocalVarDecl"; }
    LocalVarDecl(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class AssignStmt: public node {
public:
    virtual string get_node_type() { return "Assignment"; }
    AssignStmt(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class ReturnStmt: public node {
public:
    virtual string get_node_type() { return "Return"; }
    ReturnStmt(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class IfStmt: public node {
public:
    virtual string get_node_type() { return "If"; }
    IfStmt(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class ReadWriteStmt: public node {
public:
    virtual string get_node_type() { return type ? "Write" : "Read"; }
    ReadWriteStmt(int _type, int _nops, ...) { 
        type=_type;
        Nops=_nops; ops_pushback(); 
    }

    int type;
};

class Expression: public node {
public:
    virtual string get_node_type() {
        switch (op) {
            case 0:  return "Expr";
            case 1:  return "PlusExpr";
            case -1: return "MinusExpr";
            default: return "Unknown";
        }
    }
    Expression(int _op, int _nops, ...) { 
        op=_op; 
        Nops=_nops; ops_pushback(); 
    }

    // 0: none, 1: plus, -1: minus
    int op;
};

class Expressions: public node {
public:
    virtual string get_node_type() { return "Exprs"; }
    Expressions(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class ActualParams: public node {
public:
    virtual string get_node_type() { return "AcutalParams"; }
    ActualParams(int _nops, ...) { Nops=_nops; ops_pushback(); }
};

class MultiplicativeExpr: public node {
public:
    virtual string get_node_type() { 
        switch (op) {
            case 0: return "MultiplicativeExpr";
            case 1: return "MulExpr";
            case 2: return "DivExpr";
            default: return "Unknown";
        }
    }
    MultiplicativeExpr(int _op, int _nops, ...) { 
        op=_op;    
        Nops=_nops; ops_pushback(); 
    }

    // 0: none, 1: mul, -1: div
    int op;
};

class PrimaryExpr: public node {
public:
    virtual string get_node_type() { 
        switch (type) {
            case INUM: return to_string(val.ival); 
            case FNUM: return to_string(val.fval);
            case ID:   return "ID";
            case EXPR: return "Expr";
            case FUNC: return "Function";
            default:   return "Unknown";
        }
    }
    PrimaryExpr(exprtype _type, int _ival, float _fval, int _nops, ...) { 
        type=_type; 
        if(type == 0) val.ival=_ival;
        else          val.fval=_fval;
        Nops=_nops; ops_pushback(); 
    }

    // 
    exprtype type;
    union {
        int ival;
        float fval;
    } val;
};

class BoolExpression: public node {
public:
    virtual string get_node_type() { return op ? "EQ" : "NEQ"; }
    BoolExpression(int _op, int _nops, ...) { 
        op=_op;    
        Nops=_nops; ops_pushback(); 
    }

    int op;
};

// This is the class for all trivial terminals: "+", "-", etc.
class Terminal: public node {
public:
    virtual string get_node_type() { return opr; }
    Terminal(string _opr) { opr=_opr; }

    string opr;
};

// function to display the syntax tree
void print_tree_dfs(node* root, int depth, vector<int>& lineStk);

// function to eliminate left recursion on the syntax tree
void eliminate_left_recursion(node* root);