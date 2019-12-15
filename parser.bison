// Tokens
%token 
  INT 
  PLUS
  MINUS
  MULT
  DIV
  MOD
  
  EQ
  DIFF
  LESS
  GREATER
  LEQ
  GEQ

  VAR
  LET
  ATRIB
  SC
  C

  PO
  PC
  BO
  BC

  WHILE
  IF
  ELSE

  ECOM

  STR
  PRINT
  READ

  FUNC
  MAIN

  AND
  OR

// Operator associativity & precedence
%left C SC
%left ATRIB
%left OR
%left AND
%left EQ DIFF
%left IF ELSE WHILE
%left LESS LEQ GREATER GEQ
%left PLUS MINUS
%left MULT DIV MOD
%left ECOM
%left PO PC BO BC


// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  char* charValue;
  Expr* exprValue;
  BoolExpr* boolexprValue;
  AssignExpr* assignValue;
  Cmd* cmdValue;
  CmdList* cmdList;
}

%type <intValue> INT
%type <charValue> VAR
%type <charValue> STR
%type <exprValue> expr
%type <boolexprValue> boolexpr
%type <assignValue> assignexpr
%type <cmdValue> ifexpr
%type <cmdValue> whileexpr
%type <cmdValue> cmdexpr
%type <cmdList> cmdlist

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
CmdList* root;
}

%%
program: FUNC MAIN PO PC BO cmdlist BC { root = $6; }

expr: 
  INT { $$ = ast_integer($1); }
  | VAR { $$ = ast_variable($1); }
  | expr PLUS expr { $$ = ast_operation(PLUS, $1, $3); }
  | expr MINUS expr { $$ = ast_operation(MINUS, $1, $3); }
  | expr MULT expr { $$ = ast_operation(MULT, $1, $3); } 
  | expr DIV expr { $$ = ast_operation(DIV, $1, $3); }
  | expr MOD expr { $$ = ast_operation(MOD, $1, $3); }
  ;

boolexpr:
  INT { $$ = bool_ast_integer($1); }
  | expr EQ expr { $$ = bool_ast_expr(EQ, $1, $3); }
  | expr DIFF expr { $$ = bool_ast_expr(DIFF, $1, $3); }
  | expr LESS expr { $$ = bool_ast_expr(LESS, $1, $3); }
  | expr GREATER expr { $$ = bool_ast_expr(GREATER, $1, $3); }
  | expr LEQ expr { $$ = bool_ast_expr(LEQ, $1, $3); }
  | expr GEQ expr { $$ = bool_ast_expr(GEQ, $1, $3); }
  ;

assignexpr:
  LET VAR ATRIB expr { $$ = ast_assign_expr($2, $4); }
  | VAR ATRIB expr { $$ = ast_assign_expr($1, $3); }
//  | LET VAR { $$ = ast_assign_expr($2,NULL); }
  ;

ifexpr:
  IF boolexpr BO cmdlist BC { $$ = ast_if_expr($2, $4); }
  | IF boolexpr BO cmdlist BC ELSE BO cmdlist BC { $$ = ast_ifelse_expr($2, $4, $8); }
  ;  

whileexpr:
  WHILE boolexpr BO cmdlist BC { $$ = ast_while_expr($2, $4); }
  ;

cmdexpr: 
  assignexpr SC { $$ = ast_assign_cmd($1); }
  | ifexpr
  | whileexpr
  | PRINT PO STR PC SC { $$ = ast_print($3); }
  | PRINT PO VAR PC SC { $$ = ast_print($3); }
  | READ PO ECOM VAR PC SC { $$ = ast_read($4); }
  ;

cmdlist: 
  cmdexpr { $$ = ast_list_cmd($1, NULL); }
  | cmdexpr cmdlist { $$ = ast_list_cmd($1, $2); }
  ;

%%
void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}