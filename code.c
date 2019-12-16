#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "code.h"
#include "parser.h"

int idlabel = 1;

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