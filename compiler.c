#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "ast.h"
#include "compiler.h"

char temp[10];
FILE* fp;



void printInstrAux(Instr* instr) {
    switch(instr->kind) {
    case I_PLUS:
    case I_MINUS:
    case I_MULT:
    case I_DIV:
        printAtom(instr->op.el1);
        printf(",");
        printAtom(instr->op.el2);
        printf(",");
        printAtom(instr->op.el3);
        printf(",");
        printAtom(instr->op.el4);
        break;
    case I_LABEL:
    case I_GOTO:
        printAtom(instr->op.el1);
        printf(",");
        printAtom(instr->op.el2);
        printf(",");
        printAtom(instr->op.el3);
        printf(",");
        printAtom(instr->op.el4);
        break;
    case I_IFE:
    case I_IFDIF:
    case I_IFG:
    case I_IFL:
    case I_IFGE:
    case I_IFLE:
        printAtom(instr->op.el1);
        printf(",");
        printAtom(instr->op.el2);
        printf(",");
        printf("l_true,l_false");
        break;
    }
}

void printInstrMIPS(Instr* instr) {
    switch(instr->op.operator) {
    case I_PLUS:
        if(instr->op.el1->u.name[0] != '$') {
            printf("  addi ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);printf("\n");
            printf("  sw ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);
        } else {
            if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                printf("  addi ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
            } else {
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
        } else {
            if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                printf("  subi ");
                printAtom(instr->op.el1);
                printf(" ");
                printAtom(instr->op.el2);
                printf(" ");
                printAtom(instr->op.el3);
            } else {
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
        } else {
            if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                printf("  multi ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
            } else {
                printf("  mult ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
            }
        }
        break;
    case(I_DIV):
        if(instr->op.el1->u.name[0] != '$') {
            printf("  divi ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);printf("\n");
            printf("  sw ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);
        } else {
            if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                printf("  divi ");printAtom(instr->op.el1);printf(" ");printAtom(instr->op.el2);printf(" ");printAtom(instr->op.el3);
            } else {
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
        } else if(instr->op.el2->kind==A_STRING && instr->op.el2->u.name[0] != '$' && instr->op.el1->u.name[0] == '$'){
        	printf("  lw ");
            printAtom(instr->op.el1);
            printf(" ");
            printAtom(instr->op.el2);
        } else if(instr->op.el1->u.name[0] != '$') {
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

    //printInstrAux(instr);
    printf("\n");
}

void printInstr(Instr* instr) {
    printf("(");
    switch(instr->op.operator) {
    case I_PLUS:
        printf("PLUS,");
        break;
    case I_MINUS:
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

void printAtom(Atom* ex) {
    switch(ex->kind) {
    case(A_STRING):
        printf("%s", ex->u.name);
        break;
    case(A_INT):
        printf("%d", ex->u.value);
        break;
    case(A_EMPTY):
        printf(" ");
        break;
    /*case(A_LABEL):
    	printf("%s",ex->)
    	break;*/
    }
}

void printInstrList(InstrList* list) {
    printInstr(list->instr);
    if(list->next != NULL) {
        printInstrList(list->next);
    }
}

InstrList* CompileCmdList(CmdList* CmdList) {

    if(CmdList != NULL) {
        InstrList* list = compileCmd(CmdList->cmd);
        return append(list, CompileCmdList(CmdList->next));
    }
    return NULL;
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
		default:
            printf("%d\n",cmd->kind );
        /*case WHILE:
			inicio = newLabel();
			l_true = newLabel();
			l_false = newLabel();
			cond = compileBool(c->cond, l_true, l_false);
			body = compileCmd(c->cmd);
			code = [LABEL inicio] ++ cond ++ [LABEL l_true] ++ body ++ [GOTO inicio, LABEL l_false];*/
			
	}
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
        InstrList* code = CompileCmdList(root);
        printTemp(code);
        printf("  .text:\n_main:\n");
        printMIPS(code);
        //printListMips(compileExpr);
    }
    return 0;
}