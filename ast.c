// AST constructor functions
#include <stdio.h> 
#include <stdlib.h> // for malloc
#include <string.h> 
#include "ast.h" // AST header

Expr* ast_integer(int v) {
  Expr* node = (Expr*)malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr* ast_variable(char* var) {
  Expr* node = (Expr*)malloc(sizeof(Expr));
  node->kind = E_VARIABLE;
  node->attr.variable = strdup(var);
  return node;
}

Expr* ast_operation(int operator, Expr* left, Expr* right) {
  Expr* node = (Expr*)malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}


BoolExpr* bool_ast_integer(int v) {
  BoolExpr* node = (BoolExpr*)malloc(sizeof(BoolExpr));
  node->kind = BOOLINT;
  node->attr.value = v;
  return node;
}

BoolExpr* bool_ast_expr(int operator, Expr* left, Expr* right) {
  BoolExpr* node = (BoolExpr*)malloc(sizeof(BoolExpr));
  node->kind = EXPR;
  node->attr.boolop.operator = operator;
  node->attr.boolop.left = left;
  node->attr.boolop.right = right;
  return node;
}


//Atribuição
AssignExpr* ast_assign_expr(char* var, Expr* expr) {
    AssignExpr* node = (AssignExpr*)malloc(sizeof(AssignExpr));
    node->var = strdup(var);
    node->expr = expr;
    return node;
}

Cmd* ast_assign_cmd(AssignExpr* aexpr) {
  Cmd* node = (Cmd*)malloc(sizeof(Cmd));
  node->kind = C_ASSIGN;
  node->type.assign.aexpr = aexpr;
  return node;
}


Cmd* ast_if_expr(BoolExpr* bexpr, CmdList* expr) {
  Cmd* node = (Cmd*)malloc(sizeof(Cmd));
  node->kind = C_IF;
  node->type.ifthen.bexpr = bexpr;
  node->type.ifthen.expr = expr;
  return node;
}

Cmd* ast_ifelse_expr(BoolExpr* bexpr, CmdList* expr, CmdList* eexpr) {
  Cmd* node = (Cmd*)malloc(sizeof(Cmd));
  node->kind = C_IFELSE;
  node->type.ifelse.bexpr = bexpr;
  node->type.ifelse.expr = expr;
  node->type.ifelse.eexpr = eexpr;
  return node;
}

Cmd* ast_while_expr(BoolExpr* bexpr, CmdList* expr) {
  Cmd* node = (Cmd*)malloc(sizeof(Cmd));
  node->kind = C_WHILE;
  node->type.whilethen.bexpr = bexpr;
  node->type.whilethen.expr = expr;
  return node;
}

Cmd* ast_print(char* str) {
  Cmd* node = (Cmd*)malloc(sizeof(Cmd));
  node->kind = C_PRINT;
  node->type.print.str = strdup(str);
  return node;
}

Cmd* ast_read(char* var) {
  Cmd* node = (Cmd*)malloc(sizeof(Cmd));
  node->kind = C_READ;
  node->type.read.var = ast_variable(var);
  return node;
}

CmdList* ast_list_cmd(Cmd* cmd, CmdList* next) {
  CmdList* node = (CmdList*)malloc(sizeof(CmdList));
  node->cmd = cmd;
  node->next = next;
  return node;
}