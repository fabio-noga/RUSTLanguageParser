/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 85 "parser.bison" /* yacc.c:1909  */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
CmdList* root;

#line 57 "parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    PLUS = 259,
    MINUS = 260,
    MULT = 261,
    DIV = 262,
    MOD = 263,
    EQ = 264,
    DIFF = 265,
    LESS = 266,
    GREATER = 267,
    LEQ = 268,
    GEQ = 269,
    VAR = 270,
    LET = 271,
    ATRIB = 272,
    SC = 273,
    C = 274,
    PO = 275,
    PC = 276,
    BO = 277,
    BC = 278,
    WHILE = 279,
    IF = 280,
    ELSE = 281,
    ECOM = 282,
    STR = 283,
    PRINT = 284,
    READ = 285,
    FUNC = 286,
    MAIN = 287,
    AND = 288,
    OR = 289
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 62 "parser.bison" /* yacc.c:1909  */

  int intValue;
  char* charValue;
  Expr* exprValue;
  BoolExpr* boolexprValue;
  AssignExpr* assignValue;
  Cmd* cmdValue;
  CmdList* cmdList;

#line 114 "parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
