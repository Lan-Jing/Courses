%{
    #include <stdlib.h>

    #include "../include/tiny.hpp"
    #include "../include/tiny_parser.tab.hh"

    // comment out this line to suppress token output.
    #define TOKEN_OUT

#ifdef TOKEN_OUT
    int line = 1, pos = 1;
    #define yycout() \
    { \
        if(line == 1 && pos == 1) printf("Row Col: token\n"); \
        printf("%-3d,%-3d: %s\n", line, pos, yytext); \
        pos += yyleng; \
    } 
    #define yynextpos() \
    { \
        pos += yyleng; \
    }
    #define yynextline() \
    { \
        line++, pos = 1; \
    } 
#else
    #define yycout()
    #define yynextline()
    #define yynextpos()
#endif

    void yyerror(const char *s);

    // Symbol table to be added
%}

id                          [A-Za-z][A-Za-z0-9]*
int                         [0-9]+
real                        [0-9]+"."[0-9]+
newline                     \n
whitespace                  [ \t]
string                      \"([^\\\"]|\\.)*\"

%option noyywrap
%x C_COMMENT

%%

"/**"            { yycout(); BEGIN(C_COMMENT); }
<C_COMMENT>"**/" { yycout(); BEGIN(INITIAL); }
<C_COMMENT>.     { }
<C_COMMENT>\n    { }

"WRITE"                     { yycout(); yylval.nptr=new Terminal("WRITE"); return WRITE; }
"READ"                      { yycout(); yylval.nptr=new Terminal("READ"); return READ; }
"IF"                        { yycout(); yylval.nptr=new Terminal("IF"); return IF; }
"ELSE"                      { yycout(); yylval.nptr=new Terminal("ELSE"); return ELSE; }
"RETURN"                    { yycout(); yylval.nptr=new Terminal("RETURN"); return RETURN; }
"BEGIN"                     { yycout(); yylval.nptr=new Terminal("BEGIN"); return BEGINN; }
"END"                       { yycout(); yylval.nptr=new Terminal("END"); return END; }
"MAIN"                      { yycout(); yylval.nptr=new Terminal("MAIN"); return MAIN; }
"INT"                       { yycout(); return yylval.ival=INT; }
"REAL"                      { yycout(); return yylval.ival=REAL; }
"STRING"                    { yycout(); return yylval.ival=STRING; }
":="                        { yycout(); yylval.nptr=new Terminal("ASSIGN"); return ASSIGN; }
"=="                        { yycout(); yylval.nptr=new Terminal("EQ"); return EQ; }
"!="                        { yycout(); yylval.nptr=new Terminal("NEQ"); return NEQ; }

[-+*/;,()] {
    yycout();
    yylval.nptr=new Terminal(string(yytext, yyleng));
    return *yytext;
}

{id} {
    yycout();
    yylval.nptr = new Terminal(string(yytext, yyleng));
    return Id;
}

{int} {
    yycout();
    yylval.ival = atoi(yytext);
    return INum;
}

{real} {
    yycout();
    yylval.fval = atof(yytext);
    return FNum;
}

{string} {
    yycout();
    yylval.nptr = new Terminal(string(yytext, yyleng));
    return QString;
}

{newline} {
    yynextline();
}

{whitespace} {
    yynextpos();
}

. {
    yyerror("Unknown pattern");
}

%%