#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "code.h"

int counter = 0;

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

Instr* mk_instr(int operator, Atom* el1, Atom* el2, Atom* el3, Atom* el4) {
	Instr* instr = (Instr*)malloc(sizeof(Instr));
	instr->op.operator = operator;
	instr->op.el1 = el1;
	instr->op.el2 = el2;
	instr->op.el3 = el3;
	instr->op.el4 = el4;
	return instr;
}

InstrList* mk_instr_list(Instr* instr, InstrList* next) {
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
		case I_PLUS: case I_MINUS: case I_MULT: case I_DIV:
			printAtom(instr->op.el1);
			printf(",");
			printAtom(instr->op.el2);
			printf(",");
			printAtom(instr->op.el3);
			printf(",");
			printAtom(instr->op.el4);
			break;
		case I_LABEL: case I_GOTO:
			printAtom(instr->op.el1);
			printf(",");
			printAtom(instr->op.el2);
			printf(",");
			printAtom(instr->op.el3);
			printf(",");
			printAtom(instr->op.el4);
			break;
		case I_IFE: case I_IFDIF: case I_IFG: case I_IFL: case I_IFGE: case I_IFLE:
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
		case(I_PLUS):
			printf("PLUS,");
			break;
		case(I_MINUS):
			printf("MINUS,"); 
			break;
		case(I_MULT):
			printf("MULT,");
			break;
		case(I_DIV):
			printf("DIV,"); 
			break;
		case(I_ATRIB):
			printf("ATRIB,"); 
			break;
		case(I_LABEL):
			printf("LABEL,"); 
			break;
		case(I_GOTO):
			printf("GOTO,"); 
			break;
		case(I_IFE):
			printf("IFE,"); 
			break;
		case(I_IFDIF):
			printf("IFDIF,"); 
			break;
		case(I_IFG):
			printf("IFG,"); 
			break;
		case(I_IFL):
			printf("IFL,"); 
			break;
		case(I_IFGE):
			printf("IFGE,"); 
			break;
		case(I_IFLE):
			printf("IFLE,"); 
			break;
	}

	printInstrAux(instr);
	printf(")\n");
}

void printInstrList(InstrList* list) {
	printInstr(list->instr);
	if(list->next != NULL) {
		printInstrList(list->next);
	}
}


char* newVar() {
	char buffer[20];
	sprintf(buffer,"t%d", counter);
	counter++;
	return strdup(buffer);
}

int compileOp(int op) {
	
}

InstrList* compileExpr(Expr* expr, char* reg) {
	switch(expr->kind) {
		case E_INTEGER:
			char* reg = newVar();
			InstrList* code = (InstrList*)malloc(sizeof(InstrList));
			code = mk_instr_list(mk_instr(ATRIB, reg, expr->attr.value, Empty, Empty), NULL);
			return code;

		case E_VARIABLE:
			char* reg = newVar();
			InstrList* code = (InstrList*)malloc(sizeof(InstrList));
			code = mk_instr_list(mk_instr(ATRIB, reg, expr->attr.value, Empty, Empty), NULL);
			return code;

		case E_OPERATION:
			char* reg1 = newVar();
			char* reg2 = newVar();
			InstrList* list1 = (InstrList*)malloc(sizeof(InstrList));
			InstrList* list2 = (InstrList*)malloc(sizeof(InstrList));
			l1 = compileExpr(expr->left, reg1);
			l2 = compileExpr(expr->right, reg2);
			l3 = append(l1, l2);
			l4 = append(l3, mk_instr_list(mk_instr(compileOp(expr->op), reg, reg1, reg2, Empty), NULL));
			return l4;
	}
}

int main() {
	return 0;
}