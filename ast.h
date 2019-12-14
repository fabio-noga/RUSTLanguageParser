// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr {
  enum { 
    E_INTEGER,
    E_VARIABLE,
    E_OPERATION
  } kind;
  union {
    int value; // for integer values
    char* variable; //for char
    struct { 
      int operator; // PLUS, MINUS, etc 
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
  } attr;
};

struct _BoolExpr {
  enum {
    BOOLINT,
    EXPR
  } kind;
  union {
    int value;
    struct {
        int operator;
        struct _Expr* left;
        struct _Expr* right;
    } boolop;
  } attr;
};

struct _AssignExpr {
  char* var;
  struct _Expr* expr;
};

struct _Cmd {
  enum {
    C_ASSIGN,
    C_IF,
    C_IFELSE,
    C_WHILE,
    C_PRINT,
    C_READ
  } kind;
  union {
    struct {
      struct _AssignExpr* aexpr;
    } assign;
    struct {
      struct _BoolExpr* bexpr;
      struct _CmdList* expr;
    } ifthen;
    struct {
      struct _BoolExpr* bexpr;
      struct _CmdList* expr;
      struct _CmdList* eexpr;
    } ifelse;
    struct {
      struct _BoolExpr* bexpr;
      struct _CmdList* expr;
    } whilethen;
    struct {
      char* str;
    } print;
    struct {
      struct _Expr* var;
    } read;
  } type;
};

struct _CmdList {
  struct _Cmd* cmd;
  struct _CmdList* next;
};


typedef struct _Expr Expr; // Convenience typedef
typedef struct _BoolExpr BoolExpr;
typedef struct _Cmd Cmd;
typedef struct _CmdList CmdList;
typedef struct _AssignExpr AssignExpr;


// Constructor functions (see implementation in ast.c)
Expr* ast_integer(int v);
Expr* ast_variable(char* var);
Expr* ast_operation(int operator, Expr* left, Expr* right);

BoolExpr* bool_ast_integer(int v);
BoolExpr* bool_ast_expr(int operator, Expr* left, Expr* right);

AssignExpr* ast_assign_expr(char* var, Expr* expr);
Cmd* ast_assign_cmd(AssignExpr* aexpr);

Cmd* ast_if_expr(BoolExpr* bexpr, CmdList* expr);
Cmd* ast_ifelse_expr(BoolExpr* bexpr, CmdList* expr, CmdList* eexpr);
Cmd* ast_while_expr(BoolExpr* bexpr, CmdList* expr);
Cmd* ast_print(char* str);
Cmd* ast_read(char* var);

CmdList* ast_list_cmd(Cmd* cmd, CmdList* next); 

#endif