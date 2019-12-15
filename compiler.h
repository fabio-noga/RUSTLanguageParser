#ifndef __compiler_h__
#define __compiler_h__

//Funções de compilação das estruturas
char* newVar();
Instr* compileOp(Expr* expr);
Instr* compileBoolOp(BoolExpr* bexpr);
InstrList* compileExpr(Expr* expr, char* reg);
InstrList* compileBool(BoolExpr* bexpr, Atom* labelTrue, Atom* labelFalse);
InstrList* compileCmd(Cmd* cmd);
InstrList* compileCmdList(CmdList* CmdList);

#endif