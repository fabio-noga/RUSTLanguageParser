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
	return null;
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
	if(l1==NULL) return l2;
	else if(l2==NULL) return l1;
	while(temp->next != NULL) {
		p=p->next;
	}
	p->next = l2;
	return l1;
}


char* newVar() {
	char buffer[20];
	sprintf(buffer,"t%d", counter);
	counter++;
	return strdup(buffer);
}



InstrList* compileExpr(Expr* expr, char* reg) {
	switch(expr->kind) {
		case E_INTEGER:
			char* reg1 = newVar();
			InstrList* code = (InstrList*)malloc(sizeof(InstrList));
			code = mk_instr_list(mk_instr(ATRIB, reg1, expr->attr.value, Empty, Empty), NULL);
			return code;

		case E_VARIABLE:
			char* reg1 = newVar();
			InstrList* code = (InstrList*)malloc(sizeof(InstrList));
			code = mk_instr_list(mk_instr(ATRIB, reg1, expr->attr.value, Empty, Empty), NULL);
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




