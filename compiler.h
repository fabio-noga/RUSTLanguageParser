#ifndef __compiler_h__
#define __compiler_h__

InstrList* data;


char* newVar();
char* newString();
Instr* compileOp(Expr* expr);
Instr* compileBoolOp(BoolExpr* bexpr);
InstrList* compileExpr(Expr* expr, char* reg);
InstrList* compileBool(BoolExpr* bexpr, Atom* labelTrue, Atom* labelFalse);
InstrList* compileCmd(Cmd* cmd);
InstrList* CompileCmdList(CmdList* CmdList);


void printAtom(Atom* ex);
void printInstr3(Instr* instr);
void print3Adr(InstrList* list);
void printList3Adr(InstrList* list);
void printInstrMIPS(Instr* instr);
void printMIPS(InstrList* list);
void printListMips(InstrList* list);
void printDataCmd(Instr* instr);
void printData();

#endif