%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "../include/tiny.hpp"

	// comment out this line to suppress syntax tree simplification
	#define SIMPLE_TREE

	extern int yylex();
	extern int yyparse();
	extern FILE* yyin;

	void yyerror(const char *s);
%}

%require "3.2"
// %language "c++"

%define parse.lac full
%define parse.error verbose

%union {
    int ival;
    float fval;
    string* sval;
    node* nptr;
}

%token <ival> INum INT REAL STRING
%token <fval> FNum
%token <sval> QString 

// Terminals are handled as special nodes
%token <nptr> WRITE READ IF ELSE RETURN BEGINN END MAIN Id
%right <nptr> ASSIGN
%left  <nptr> EQ NEQ 
%left  <nptr> '+' '-' '*' '/'
%token <nptr> ';' ',' '(' ')'

// Nonterminals inherit from super class node
%type <nptr> Program
%type <nptr> Type
%type <nptr> MethodDecls
%type <nptr> MethodDecl
%type <nptr> FormalParams
%type <nptr> Block
%type <nptr> Statements 
%type <nptr> Statement
%type <nptr> LocalVarDecl
%type <nptr> AssignStmt
%type <nptr> ReturnStmt
%type <nptr> IfStmt
%type <nptr> ReadStmt
%type <nptr> WriteStmt
%type <nptr> Expression
%type <nptr> Expressions
%type <nptr> ActualParams
%type <nptr> MultiplicativeExpr
%type <nptr> PrimaryExpr
%type <nptr> BoolExpression

%start Program

%%

Program:  
		MethodDecls											{ 
			$$=new Program(1, $1); 
		#ifdef SIMPLE_TREE
            eliminate_left_recursion($$);
        #endif
		    vector<int> lineStk; print_tree_dfs($$, 0, lineStk); 
            delete $$;
		}
		;

MethodDecls:
        MethodDecl                                          { $$=new MethodDecls(1, $1);  }
    |   MethodDecls MethodDecl                              { $$=new MethodDecls(2, $1, $2); }
        ;

Type:
        INT                                                 { $$=new Type(0); }
    |   REAL                                                { $$=new Type(1); }
    |   STRING                                              { $$=new Type(2); }
	| 	error
        ;

MethodDecl:
        Type MAIN Id '(' FormalParams ')' Block             { $$=new MethodDecl(7, $1, $2, $3, $4, $5, $6, $7); }
    |   Type      Id '(' FormalParams ')' Block             { $$=new MethodDecl(6, $1, $2, $3, $4, $5, $6); }
    |   Type MAIN Id '('              ')' Block             { $$=new MethodDecl(6, $1, $2, $3, $4, $5, $6); }
    |   Type      Id '('              ')' Block             { $$=new MethodDecl(5, $1, $2, $3, $4, $5); }
        ;

FormalParams:
        Type Id                                             { $$=new FormalParams(2, $1, $2); }
    |   FormalParams ',' Type Id                            { $$=new FormalParams(4, $1, $2, $3, $4); }
	|	FormalParams ',' error Id
        ;

Block:
        BEGINN Statements END                               { $$=new Block(3, $1, $2, $3); }
        ;

Statements:
        Statement                                           { $$=new Statements(1, $1); }
    |   Statements Statement                                { $$=new Statements(2, $1, $2); }
        ;

Statement:
        Block                                               { $$=new Statement(BLOCK, 1, $1); }
    |   LocalVarDecl                                        { $$=new Statement(LOCALVARDECL, 1, $1); }
    |   AssignStmt                                          { $$=new Statement(ASSIGNSTMT, 1, $1); }
    |   ReturnStmt                                          { $$=new Statement(RETURNSTMT, 1, $1); }
    |   IfStmt                                              { $$=new Statement(IFSTMT, 1, $1); }
    |   WriteStmt                                           { $$=new Statement(WRITESTMT, 1, $1); }
    |   ReadStmt                                            { $$=new Statement(READSTMT, 1, $1); }
        ;

LocalVarDecl:
        Type Id ';'                                         { $$=new LocalVarDecl(3, $1, $2, $3); }
    |   Type AssignStmt                                     { $$=new LocalVarDecl(2, $1, $2); }
	|	error Id ';'
	|	Type error ';'
        ;

AssignStmt:
        Id ASSIGN Expression ';'                            { $$=new AssignStmt(4, $1, $2, $3, $4); }
    |   Id ASSIGN QString ';'                               { $$=new AssignStmt(4, $1, $2, $3, $4); }
        ;

ReturnStmt:
        RETURN Expression ';'                               { $$=new ReturnStmt(3, $1, $2, $3); }
        ;

IfStmt:
        IF '(' BoolExpression ')' Statement                 { $$=new IfStmt(5, $1, $2, $3, $4, $5); }
    |   IF '(' BoolExpression ')' Statement ELSE Statement  { $$=new IfStmt(7, $1, $2, $3, $5, $5, $6, $7); }
        ;

WriteStmt:
        WRITE '(' Expression ',' QString ')' ';'            { $$=new ReadWriteStmt(1, 7, $1, $2, $3, $4, $5, $6, $7); }
        ;

ReadStmt:
        READ '(' Id ',' QString ')' ';'                     { $$=new ReadWriteStmt(0, 7, $1, $2, $3, $4, $5, $6, $7); }
        ;

Expression:
        MultiplicativeExpr                                  { $$=new Expression(0, 1, $1); }
    |   Expression '+' MultiplicativeExpr                   { $$=new Expression(1, 3, $1, $2, $3); }
    |   Expression '-' MultiplicativeExpr                   { $$=new Expression(-1, 3, $1, $2, $3); }
        ;

PrimaryExpr:
        INum                                                { $$=new PrimaryExpr(INUM, $1, 0, 0); }
    |   FNum                                                { $$=new PrimaryExpr(FNUM, 0, $1, 0); }
    |   Id                                                  { $$=new PrimaryExpr(ID, 0, 0, 1, $1); }
    |   '(' Expression ')'                                  { $$=new PrimaryExpr(EXPR, 0, 0, 3, $1, $2, $3); }
    |   Id '(' ActualParams ')'                             { $$=new PrimaryExpr(FUNC, 0, 0, 4, $1, $2, $3, $4); }
    ;

MultiplicativeExpr:
        PrimaryExpr                                         { $$=new MultiplicativeExpr(0, 1, $1); }
    |   MultiplicativeExpr '*' PrimaryExpr                  { $$=new MultiplicativeExpr(1, 3, $1, $2, $3); }
    |   MultiplicativeExpr '/' PrimaryExpr                  { $$=new MultiplicativeExpr(-1, 3, $1, $2, $3); }
        ; 

BoolExpression:
        Expression EQ Expression                            { $$=new BoolExpression(1, 3, $1, $2, $3); }
    |   Expression NEQ Expression                           { $$=new BoolExpression(0, 3, $1, $2, $3); }
        ;

ActualParams:
        %empty                                              { $$=new ActualParams(0); }
    |   Expressions                                         { $$=new ActualParams(1, $1); }
        ;

Expressions:
        Expression                                          { $$=new Expressions(1, $1); }
    |   Expressions ',' Expression                          { $$=new Expressions(3, $1, $2, $3); }
        ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
    return ;
}

int main(int argc, char* argv[]) 
{
	if(argc > 1) {
		yyin = fopen(argv[1], "r");
	} else {
		yyin = stdin;
	}

    yyparse();
    return 0;
}