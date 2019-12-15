#ifndef __code_h__
#define __code_h__

struct _Atom {
	enum {
		A_INT, A_STRING, A_EMPTY, A_LABEL
	} kind;
	union {
		int value;
		char* name;
	} u;
};

struct _Label {
	int idlabel;
};

struct _Instr {
	enum {
		I_PLUS, 
		I_MINUS, 
		I_MULT, 
		I_DIV, 
		I_ATRIB, 
		I_LABEL, 
		I_GOTO, 
		I_IFE, 
		I_IFDIF, 
		I_IFG, 
		I_IFL, 
		I_IFGE, 
		I_IFLE, 
		I_PRINT, 
		I_SCAN
	} kind;
	struct {
		int operator;
		struct _Atom* el1;
		struct _Atom* el2;
		struct _Atom* el3;
		struct _Atom* el4;
	} op;
};

struct _InstrList {
	struct _Instr* instr;
	struct _InstrList* next;
};


typedef struct _Atom Atom;
typedef struct _Instr Instr;
typedef struct _InstrList InstrList;
typedef struct _Label Label;


Atom* atom_integer(int v);
Atom* atom_variable(char* name);
Atom* atom_empty();
Atom* atom_label();
Instr* mk_instr(int operator, Atom* el1, Atom* el2, Atom* el3, Atom* el4);
InstrList* mk_instr_list(Instr* instr, InstrList* next);
Label* mk_label(int label_id);


Instr* getFirst(InstrList* list);
InstrList* nextInstrs(InstrList* list);
InstrList* append(InstrList* l1, InstrList* l2);


void printAtom(Atom* ex);
void printInstrAux(Instr* instr);
void printInstr(Instr* instr);
void printInstrList(InstrList* list);


char* newVar();
Instr* compileOp(Expr* expr);
InstrList* compileExpr(Expr* expr, char* reg);
InstrList* compileBool(BoolExpr* bexpr, Atom* labelTrue, Atom* labelFalse);

Instr* compileBoolOp(BoolExpr* bexpr);
//InstrList* compileBool(BoolExpr bexpr, Label* labelTrue, Label* labelFalse);


#endif