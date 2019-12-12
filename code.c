#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "code.h"

Atom* atom_integer(int v) {
	Atom* a = (Atom*)malloc(sizeof(Atom));
	a->kind = A_INT;
	a->u.value = v;
	return a;
}

Atom* atom_variable(char* var) {
	Atom* a = (Atom*)malloc(sizeof(Atom));
	a->kind = A_STRING;
	a->u.name = strdup(var);
	return a;
}

Atom* atom_empty() {
	Atom* a = (Atom*)malloc(sizeof(Atom));
	a->kind = A_EMPTY;
	return a;
}

Instr* instr(int operator, Atom* el1, Atom* el2, Atom* el3, Atom* el4) {
	Instr* instr = (Instr*)malloc(sizeof(Instr));
	instr->op.operator = operator;
	instr->op.el1 = el1;
	instr->op.el2 = el2;
	instr->op.el3 = el3;
	instr->op.el4 = el4;
	return instr;
}

InstrList* instr_list(Instr* instr, InstrList* next) {
	InstrList* list = (InstrList*)malloc(sizeof(InstrList));
	list->instr = instr;
	list->next = next;
	return list;
}


Instr* getFirst(InstrList* list) {
	return list->instr;
}

InstrList* nextInstrs(InstrList* list) {
	return list->next;
}

InstrList* append(InstrList* l1, InstrList* l2) {
	InstrList* temp = l1;
	while(l1->next != NULL) {
		l1 = l1->next;
	}
	l1->next = l2;
	return temp;
}


void printAtom(Atom* ex) {
	switch(ex->kind) {
		case(A_STRING):
			printf("%s", ex->u.name);
			break;
		case(A_INT):
			printf("%d", ex->u.value);
			break;
		case(A_EMPTY):
			printf("empty");
			break;
	}
}

void printInstrAux(Instr* instr) {
	switch(instr->kind) {
		case PLUS: case MINUS: case MULT: case DIV:
			printAtom(instr->op.el1);
			printf(",");
			printAtom(instr->op.el2);
			printf(",");
			printAtom(instr->op.el3);
			printf(",");
			printAtom(instr->op.el4);
			break;
		case LABEL: case GOTO:
			printAtom(instr->op.el1);
			printf(",");
			printAtom(instr->op.el2);
			printf(",");
			printAtom(instr->op.el3);
			printf(",");
			printAtom(instr->op.el4);
			break;
		case IFE: case IFDIF: case IFG: case IFL: case IFGE: case IFLE:
			printAtom(instr->op.el1);
			printf(",");
			printAtom(instr->op.el2);
			printf(",");
			printf("l_true,l_false");
			break;
	}
}

void printInstr(Instr* instr) {
	printf("(");
	switch(instr->kind) {
		case(PLUS):
			printf("PLUS,");
			break;
		case(MINUS):
			printf("MINUS,"); 
			break;
		case(MULT):
			printf("MULT,");
			break;
		case(DIV):
			printf("DIV,"); 
			break;
		case(ATRIB):
			printf("ATRIB,"); 
			break;
		case(LABEL):
			printf("LABEL,"); 
			break;
		case(GOTO):
			printf("GOTO,"); 
			break;
		case(IFE):
			printf("IFE,"); 
			break;
		case(IFDIF):
			printf("IFDIF,"); 
			break;
		case(IFG):
			printf("IFG,"); 
			break;
		case(IFL):
			printf("IFL,"); 
			break;
		case(IFGE):
			printf("IFGE,"); 
			break;
		case(IFLE):
			printf("IFLE,"); 
			break;
	}

	printInstrAux(instr);
	printf(")\n");
}

void printListInstr(InstrList* list) {
	printInstr(list->instr);
	if(list->next != NULL) {
		printListInstr(list->next);
	}
}


/*InstrList compileExpr(Expr e, char *r) {
	//...
	switch(e->kind)
	{
		case NUM:
			code = mkInstrList(mkInstr(ATRIB, reg, e->value, Empty, Empty), NULL);
			return code;
		break;

		case VAR:

		break;

		case ExprOp:
		r1 = newVar();
		l1 = compileExpr(e->left, r1);
		r2 = newVer();
		l2 = compileExpr(e->right, r2);
		l3 = append(l1, l2);
		l4 = append(l3, mkInstrList(mkInstr(comp(e->op), r, r1, r2, Empty), NULL));

		return l4;
	}
}*/

int main() {
	return 0;
}