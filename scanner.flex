%{
// HEADERS
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%

[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

"+" { return PLUS; }
"-" { return MINUS; }
"*" { return MULT; }
"/" { return DIV; }
"%" { return MOD; }

"==" { return EQ; }
"!=" { return DIFF; }
"<" { return LESS; }
">" { return GREATER; }
"<=" { return LEQ; }
">=" { return GEQ; }

"let" { return LET; }
"=" { return ATRIB; }
";" { return SC; }
"," { return C; }

"(" { return PO; }
")" { return PC; }
"{" { return BO; }
"}" { return BC; }

"while" { return WHILE; }
"if" { return IF; }
"else" { return ELSE; }

"& " { return ECOM; }

"println!" { return PRINT; }
"read_line" { return READ; }

"fn" { return FUNC; }
"main" { return MAIN; }

\-?[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}

[_a-zA-Z]+[_0-9]*[_a-zA-Z0-9]* { 
   yylval.charValue = strdup(yytext);
   return VAR; 
}

\".*\" { 
	yylval.charValue = strdup(yytext);
   return STR;  
}

.  { yyerror("unexpected character"); }
%%