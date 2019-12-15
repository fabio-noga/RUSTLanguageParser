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
InstrList* data;

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
	switch(bexpr->kind) {
		case BOOLINT:
			return mk_instr_list(mk_instr(I_ATRIB, atom_variable(newVar()), atom_integer(bexpr->attr.value), atom_empty(), atom_empty()), NULL);
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

InstrList* compileCmd(Cmd* cmd) {
    InstrList* l1=(InstrList*)malloc(sizeof(InstrList));
    InstrList* l2=(InstrList*)malloc(sizeof(InstrList));
    InstrList* l3=(InstrList*)malloc(sizeof(InstrList));
    InstrList* l4=(InstrList*)malloc(sizeof(InstrList));
    switch(cmd->kind) {
        case C_ASSIGN:
            return compileExpr((cmd->type.assign.aexpr)->expr,(cmd->type.assign.aexpr)->var);
        case C_IF:;
            Atom* lb3=atom_label();
            l1 = compileBool(cmd->type.ifelse.bexpr, lb3, atom_empty());
            l2 = CompileCmdList(cmd->type.ifelse.expr);
            l2 = append(l1,l2);
            return append(l2,mk_instr_list(mk_instr(I_LABEL,lb3,atom_empty(),atom_empty(),atom_empty()),NULL));
        case C_IFELSE:;
            Atom* lb1=atom_label();
            Atom* lb2=atom_label();
            l1 = compileBool(cmd->type.ifelse.bexpr, lb1, lb2);
            l2 = CompileCmdList(cmd->type.ifelse.expr);
            l4 = append(l1,l2);
            //goto out of then
            l4 = append(l4,mk_instr_list(mk_instr(I_GOTO,lb2,atom_empty(),atom_empty(),atom_empty()),NULL));
            l4 = append(l4,mk_instr_list(mk_instr(I_LABEL,lb1,atom_empty(),atom_empty(),atom_empty()),NULL));
            l3 = CompileCmdList(cmd->type.ifelse.eexpr);
            l4 = append(l4,l3);
            return append(l4,mk_instr_list(mk_instr(I_LABEL,lb2,atom_empty(),atom_empty(),atom_empty()),NULL));
        
        case C_WHILE:
            ;
            lb1 = atom_label();
            lb2 = atom_label();
            l1 = compileBool(cmd->type.whilethen.bexpr, lb2, atom_empty());
            l2 = CompileCmdList(cmd->type.whilethen.expr);
            l3 = append(mk_instr_list(mk_instr(I_LABEL, lb1, atom_empty(), atom_empty(), atom_empty()), NULL), l1);
            l4 = append(l3, l2);
            l4 = append(l4, mk_instr_list(mk_instr(I_GOTO, lb1, atom_empty(), atom_empty(), atom_empty()), NULL));
            return append(l3, mk_instr_list(mk_instr(I_LABEL, lb2, atom_empty(), atom_empty(), atom_empty()), NULL));
            
        case C_PRINT:
            //fprintf(fp,"-----------%s\n", cmd->type.print.str);
            if(isdigit(cmd->type.print.str[0]))
                return mk_instr_list(mk_instr(I_PRINT,atom_variable(cmd->type.print.str),atom_integer(1),atom_variable(cmd->type.print.str),atom_empty()),NULL);
            
            if(cmd->type.print.str[0]=='\'' || cmd->type.print.str[0]=='\"'){
                char* temp=newString();
                l1=mk_instr_list(mk_instr(I_PRINT,atom_variable(temp),atom_variable(cmd->type.print.str),atom_empty(),atom_empty()),NULL);
                data=append(data,l1);
                return mk_instr_list(mk_instr(I_PRINT,atom_variable(temp),atom_integer(4),atom_variable(cmd->type.print.str),atom_empty()),NULL);
            }
            return mk_instr_list(mk_instr(I_PRINT,atom_variable(cmd->type.print.str),atom_integer(4),atom_variable(cmd->type.print.str),atom_empty()),NULL);
            
            //4-String 1-Int 
            
            
        case C_READ:
            return mk_instr_list(mk_instr(I_SCAN,atom_variable(cmd->type.read.var->attr.variable),atom_variable(newVar()),atom_empty(),atom_empty()),NULL);
    }
}


InstrList* CompileCmdList(CmdList* CmdList) {

    if(CmdList != NULL) {
        InstrList* list = compileCmd(CmdList->cmd);
        return append(list, CompileCmdList(CmdList->next));
    }
    return NULL;
}

