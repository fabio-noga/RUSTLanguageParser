#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "ast.h"
#include "compiler.h"

int counter = 0;
char temp[10];
FILE* fp;

char* newVar() {
    char buffer[20];
    sprintf(buffer, "$t%d", counter);
    counter++;
    return strdup(buffer);
}

Instr* compileOp(Expr* expr) {
    Instr* instr = (Instr*)malloc(sizeof(Instr));
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

InstrList* compileExpr(Expr* expr, char* reg) {
    switch(expr->kind) {
        case E_INTEGER:
            return mk_instr_list(mk_instr(I_ATRIB, atom_variable(reg), atom_integer(expr->attr.value), atom_empty(), atom_empty()), NULL);

        case E_VARIABLE:
            return mk_instr_list(mk_instr(I_ATRIB, atom_variable(reg), atom_variable(expr->attr.variable), atom_empty(), atom_empty()), NULL);

        case E_OPERATION:
            ;
            char* reg1 = newVar();
            char* reg2 = newVar();
            InstrList* l1 = compileExpr(expr->attr.op.left, reg1);
            InstrList* l2 = compileExpr(expr->attr.op.right, reg2);
            InstrList* l3 = append(l1, l2);
            return append(l3, mk_instr_list(mk_instr(compileOp(expr)->kind, atom_variable(reg), atom_variable(reg1), atom_variable(reg2), atom_empty()), NULL));
        }
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

InstrList* compileCmd(Cmd* cmd) {
    InstrList* l1 = (InstrList*)malloc(sizeof(InstrList));
    InstrList* l2 = (InstrList*)malloc(sizeof(InstrList));
    InstrList* l3 = (InstrList*)malloc(sizeof(InstrList));
    InstrList* l4 = (InstrList*)malloc(sizeof(InstrList));
    Atom* lb1 = (Atom*)malloc(sizeof(Atom));
    Atom* lb2 = (Atom*)malloc(sizeof(Atom));
    Atom* lb3 = (Atom*)malloc(sizeof(Atom));
    switch(cmd->kind) {
        case C_ASSIGN:
            return compileExpr((cmd->type.assign.aexpr)->expr,(cmd->type.assign.aexpr)->var);
        case C_IF:
            ;
            lb1 = atom_label();
            l1 = compileBool(cmd->type.ifelse.bexpr, lb3, atom_empty());
            l2 = compileCmdList(cmd->type.ifelse.expr);
            l3 = append(l1, l2);
            return append(l3, mk_instr_list(mk_instr(I_LABEL, lb1, atom_empty(), atom_empty(), atom_empty()), NULL));
        case C_IFELSE:
            ;
            lb1 = atom_label();
            lb2 = atom_label();
            l1 = compileBool(cmd->type.ifelse.bexpr, lb1, lb2);
            l2 = compileCmdList(cmd->type.ifelse.expr);
            l3 = append(l1, l2);
            //goto out of then
            l4 = append(l3, mk_instr_list(mk_instr(I_GOTO, lb1, atom_empty(), atom_empty(), atom_empty()), NULL));
            l4 = append(l4, mk_instr_list(mk_instr(I_LABEL, lb2, atom_empty(), atom_empty(), atom_empty()), NULL));
            l3 = compileCmdList(cmd->type.ifelse.eexpr);
            l4 = append(l4,l3);
            return append(l4, mk_instr_list(mk_instr(I_LABEL, lb1, atom_empty(), atom_empty(), atom_empty()), NULL));
        case C_WHILE:
            ;
            lb1 = atom_label();
            lb2 = atom_label();
            l1 = compileBool(cmd->type.whilethen.bexpr, lb2, atom_empty());
            l2 = compileCmdList(cmd->type.whilethen.expr);
            l3 = append(mk_instr_list(mk_instr(I_LABEL, lb1, atom_empty(), atom_empty(), atom_empty()), NULL), l1);
            l4 = append(l3, l2);
            l4 = append(l4, mk_instr_list(mk_instr(I_GOTO, lb1, atom_empty(), atom_empty(), atom_empty()), NULL));
            return append(l3, mk_instr_list(mk_instr(I_LABEL, lb2, atom_empty(), atom_empty(), atom_empty()), NULL));
        default:
            printf("%d\n",cmd->kind);
    }
}

InstrList* compileCmdList(CmdList* CmdList) {
    if(CmdList != NULL) {
        InstrList* list = compileCmd(CmdList->cmd);
        return append(list, compileCmdList(CmdList->next));
    }
    return NULL;
}


void printInstrMIPS(Instr* instr) {
    switch(instr->op.operator) {
        case I_PLUS:
            if(instr->op.el1->u.name[0] != '$') {
                printf("  addi ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);printf("\n");
                printf("  sw ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);
            }
            else {
                if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                    printf("  addi ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
                }
                else {
                    printf("  add ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
                }
            }
            break;
        case I_MINUS:
            if(instr->op.el1->u.name[0] != '$') {
                printf("  subi ");
                printAtom(instr->op.el2);
                printf(" ");
                printAtom(instr->op.el2);
                printf(" ");
                printAtom(instr->op.el3);
                printf("\n");
                printf("  sw ");
                printAtom(instr->op.el1);
                printf(" ");
                printAtom(instr->op.el2);
            }
            else {
                if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                    printf("  subi ");
                    printAtom(instr->op.el1);
                    printf(" ");
                    printAtom(instr->op.el2);
                    printf(" ");
                    printAtom(instr->op.el3);
                }
                else {
                    printf("  sub ");
                    printAtom(instr->op.el1);
                    printf(" ");
                    printAtom(instr->op.el2);
                    printf(" ");
                    printAtom(instr->op.el3);
                }
            }
            break;
        case(I_MULT):
            if(instr->op.el1->u.name[0] != '$') {
                printf("  multi ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);printf("\n");
                printf("  sw ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);
            }
            else {
                if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                    printf("  multi ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
                }
                else {
                    printf("  mult ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
                }
            }
            break;
        case(I_DIV):
            if(instr->op.el1->u.name[0] != '$') {
                printf("  divi ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);printf("\n");
                printf("  sw ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);
            }
            else {
                if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                    printf("  divi ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
                }
                else {
                    printf("  div ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
                }
            }
            break;
        case(I_ATRIB):
            if(instr->op.el2->kind == A_INT && instr->op.el1->u.name[0] == '$') {
                printf("  li ");
                printAtom(instr->op.el1);
                printf(" ");
                printAtom(instr->op.el2);
            }
            else if(instr->op.el2->kind==A_STRING && instr->op.el2->u.name[0] != '$' && instr->op.el1->u.name[0] == '$'){
            	printf("  lw ");
                printAtom(instr->op.el1);
                printf(" ");
                printAtom(instr->op.el2);
            }
            else if(instr->op.el1->u.name[0] != '$') {
                printf("  sw ");
                printAtom(instr->op.el1);
                printf(" ");
                printAtom(instr->op.el2);
            }
            break;
        case(I_LABEL):
            printf("%s:",instr->op.el1->u.name);
            break;
        case(I_GOTO):
            printf("  j ");
            printf("%s",instr->op.el1->u.name);
            //printAtom(instr->op.el1);
            break;
        case(I_IFE):
            printf("  bne ");
            printAtom(instr->op.el1);
            printf(" ");
            printAtom(instr->op.el2);
            printf(" ");
            printAtom(instr->op.el3);
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
    printf("\n");
}


void printListMips(CmdList* list, int tabs) {
    char fileName[25] = "exampleMips.txt";
    fp = fopen(fileName, "w");
    if ( fp == NULL ) {
        printf("Impossivel criar arquivo Mips\n");
        return;
    }
    //hash
    //fprintf(fp, ".data\n");

    fprintf(fp, ".text\n");
    fprintf(fp, "main:\n");

    if(list != NULL) {
        //printCmd(list->cmd, tabs);
        //printList(list->next, tabs);
    }
}

void printTemp(InstrList* list) {
    printInstr(list->instr);
    if(list->next != NULL)printTemp(list->next);
}

void printMIPS(InstrList* list) {
    printInstrMIPS(list->instr);
    if(list->next != NULL)printMIPS(list->next);
    else printf("  syscall\n");
}


int main(int argc, char** argv) {
    --argc;
    ++argv;
    if (argc != 0) {
        yyin = fopen(*argv, "r");
        if (!yyin) {
            printf("'%s': could not open file\n", *argv);
            return 1;
        }
    }
    if (yyparse() == 0) {
        //InstrList* result = compileExpr(root);
        //CompileCmdList(root);
        InstrList* code = compileCmdList(root);
        printTemp(code);
        printf("  .text:\n_main:\n");
        printMIPS(code);
        //printListMips(compileExpr);
    }
    return 0;
}