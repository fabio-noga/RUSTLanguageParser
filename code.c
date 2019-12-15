#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "code.h"
#include "parser.h"

int counter = 0;
int counter1 = 0;
int idlabel = 1;
int sentinel=0;

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

Atom* atom_label() {
    Atom* a = (Atom*)malloc(sizeof(Atom));
    a->kind = A_LABEL;
    char lbuffer[20];
    sprintf(lbuffer, "L%d", idlabel);
    idlabel++;
    a->u.name=lbuffer;
    return atom_variable(strdup(lbuffer));
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

Label* mk_label(int idlabel) {
    Label* label = (Label*)malloc(sizeof(Label));
    label->idlabel = idlabel;
    return label;
}




Instr* getFirst(InstrList* list) {
    return list->instr;
}

InstrList* nextInstrs(InstrList* list) {
    return list->next;
}

InstrList* append(InstrList* l1, InstrList* l2) {
    InstrList* temp = l1;
    if(l1 == NULL)
        return l2;
    else if(l2 == NULL)
        return l1;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = l2;
    return l1;
}


char* newVar() {
    char buffer[20];
    sprintf(buffer, "$t%d", counter);
    counter++;
    return strdup(buffer);
}

char* newString() {
    char buffer[20];
    sprintf(buffer, "prompt%d", counter1);
    counter1++;
    return strdup(buffer);
}
Instr* compileBoolOp(BoolExpr* bexpr) {
	Instr* instr = (Instr*)malloc(sizeof(Instr));
	switch(bexpr->attr.boolop.operator) {
		case EQ:
            instr->kind = I_IFE;
            return instr;
        case DIFF:
            instr->kind = I_IFDIF;
            return instr;
        case GREATER:
            instr->kind = I_IFG;
            return instr;
        case LESS:
            instr->kind = I_IFL;
            return instr;
        case GEQ:
            instr->kind = I_IFGE;
            return instr;
        case LEQ:
            instr->kind = I_IFLE;
            return instr;
	}
}
Instr* compileOp(Expr* expr) {
    Instr* instr = (Instr*)malloc(sizeof(Instr));
    //if(expr->kind == E_OPERATION) {
        switch(expr->attr.op.operator) {
        case PLUS:
            instr->kind = I_PLUS;
            return instr;
        case MINUS:
            instr->kind = I_MINUS;
            return instr;
        case MULT:
            instr->kind = I_MULT;
            return instr;
        case DIV:
            instr->kind = I_DIV;
            return instr;
        }
    //}
}

InstrList* compileBool(BoolExpr* bexpr, Atom* labelTrue, Atom* labelFalse) {
	char* reg = newVar();
	switch(bexpr->kind) {
		case BOOLINT:
			return mk_instr_list(mk_instr(I_ATRIB, atom_variable(reg), atom_integer(bexpr->attr.value), atom_empty(), atom_empty()), NULL);
		case EXPR:
			;
			char* reg1 = newVar();
	        char* reg2 = newVar();
	        InstrList* l1 = compileExpr(bexpr->attr.boolop.left, reg1);
	        InstrList* l2 = compileExpr(bexpr->attr.boolop.right, reg2);
	        InstrList* l3 = append(l1, l2);
	        return append(l3, mk_instr_list(mk_instr(compileBoolOp(bexpr)->kind, atom_variable(reg1), atom_variable(reg2), labelTrue, labelFalse), NULL));
	}
}

InstrList* compileExpr(Expr* expr, char* reg) {
    InstrList* code = (InstrList*)malloc(sizeof(InstrList));
    switch(expr->kind) {
    case E_INTEGER:
        code = mk_instr_list(mk_instr(I_ATRIB, atom_variable(reg), atom_integer(expr->attr.value), atom_empty(), atom_empty()), NULL);
        return code;

    case E_VARIABLE:
        code = mk_instr_list(mk_instr(I_ATRIB, atom_variable(reg), atom_variable(expr->attr.variable), atom_empty(), atom_empty()), NULL);
        return code;

    case E_OPERATION:
        ;
        char* reg1 = newVar();
        char* reg2 = newVar();
        InstrList* l1 = (InstrList*)malloc(sizeof(InstrList));
        InstrList* l2 = (InstrList*)malloc(sizeof(InstrList));
        l1 = compileExpr(expr->attr.op.left, reg1);
        l2 = compileExpr(expr->attr.op.right, reg2);
        InstrList* l3 = append(l1, l2);
        Instr* temp=compileOp(expr);
        InstrList* l4 = append(l3, mk_instr_list(mk_instr(temp->kind, atom_variable(reg), atom_variable(reg1), atom_variable(reg2), atom_empty()), NULL));
        return l4;
    }
}

/*InstrList* compileBool(BoolExpr bexpr, Label* labelTrue, Label* labelFalse) {

}*/