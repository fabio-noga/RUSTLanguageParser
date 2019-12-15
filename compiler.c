#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "ast.h"
#include "compiler.h"

char temp[10];
FILE* fp;

void printInstrMIPS(Instr* instr) {
    switch(instr->op.operator) {
    case I_PLUS:
        if(instr->op.el1->u.name[0] != '$') {
            fprintf(fp,"  add ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);fprintf(fp,"\n");
            fprintf(fp,"  sw ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);
        } else {
            if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                fprintf(fp,"  addi ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);
            } else {
                fprintf(fp,"  add ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);
            }
        }
        break;
    case I_MINUS:
        if(instr->op.el1->u.name[0] != '$') {
            fprintf(fp,"  sub ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);fprintf(fp,"\n");
            fprintf(fp,"  sw ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);
        } else {
            if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                fprintf(fp,"  subi ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);
            } else {
                fprintf(fp,"  sub ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);
            }
        }
        break;
    case(I_MULT):
        if(instr->op.el1->u.name[0] != '$') {
            fprintf(fp,"  mult ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);fprintf(fp,"\n");
            fprintf(fp,"  sw ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);
        } else {
            if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                fprintf(fp,"  multi ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);
            } else {
                fprintf(fp,"  mult ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);
            }
        }
        break;
    case(I_DIV):
        if(instr->op.el1->u.name[0] != '$') {
            fprintf(fp,"  div ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);fprintf(fp,"\n");
            fprintf(fp,"  sw ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);
        } else {
            if(instr->op.el1->u.name[0] != '$' || instr->op.el2->u.name[0] != '$' || instr->op.el3->u.name[0] != '$') {
                fprintf(fp,"  divi ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);
            } else {
                fprintf(fp,"  div ");printAtom(instr->op.el1);fprintf(fp," ");printAtom(instr->op.el2);fprintf(fp," ");printAtom(instr->op.el3);
            }
        }
        break;
    case(I_ATRIB):
        if(instr->op.el2->kind == A_INT && instr->op.el1->u.name[0] == '$') {
            fprintf(fp,"  li ");
            printAtom(instr->op.el1);
            fprintf(fp," ");
            printAtom(instr->op.el2);
        } else if(instr->op.el2->kind==A_STRING && instr->op.el2->u.name[0] != '$' && instr->op.el1->u.name[0] == '$'){
        	fprintf(fp,"  lw ");
            printAtom(instr->op.el1);
            fprintf(fp," ");
            printAtom(instr->op.el2);
        } else if(instr->op.el1->u.name[0] != '$') {
            fprintf(fp,"  sw ");
            printAtom(instr->op.el1);
            fprintf(fp," ");
            printAtom(instr->op.el2);
        }
        break;
    case(I_LABEL):
        fprintf(fp,"%s:",instr->op.el1->u.name);
        break;
    case(I_GOTO):
        fprintf(fp,"  j ");
        fprintf(fp,"%s",instr->op.el1->u.name);
        //printAtom(instr->op.el1);
        break;
    case(I_IFE):
        fprintf(fp,"  bne ");
        printAtom(instr->op.el1);
        fprintf(fp," ");
        printAtom(instr->op.el2);
        fprintf(fp," ");
        printAtom(instr->op.el3);
        break;
    case(I_IFDIF):
        fprintf(fp,"  beq ");
        printAtom(instr->op.el1);
        fprintf(fp," ");
        printAtom(instr->op.el2);
        fprintf(fp," ");
        printAtom(instr->op.el3);
        break;
    case(I_IFG):
        fprintf(fp,"  ble ");
        printAtom(instr->op.el1);
        fprintf(fp," ");
        printAtom(instr->op.el2);
        fprintf(fp," ");
        printAtom(instr->op.el3);
        break;
    case(I_IFL):
        fprintf(fp,"  bge ");
        printAtom(instr->op.el1);
        fprintf(fp," ");
        printAtom(instr->op.el2);
        fprintf(fp," ");
        printAtom(instr->op.el3);
        break;
    case(I_IFGE):
        fprintf(fp,"  blt ");
        printAtom(instr->op.el1);
        fprintf(fp," ");
        printAtom(instr->op.el2);
        fprintf(fp," ");
        printAtom(instr->op.el3);
        break;
    case(I_IFLE):
        fprintf(fp,"  bgt ");
        printAtom(instr->op.el1);
        fprintf(fp," ");
        printAtom(instr->op.el2);
        fprintf(fp," ");
        printAtom(instr->op.el3);
        break;
    case(I_PRINT):
        fprintf(fp,"  li $v0, %d\n", instr->op.el2->u.value);
        //if(instr->op.el2->u.value==4){
            fprintf(fp,"  la $a0, ");printAtom(instr->op.el1);
        //}
        fprintf(fp,"\n  syscall");
        break;
    case(I_SCAN):
        fprintf(fp,"  li $v0, 5\n");
        fprintf(fp,"  la $a0, ");printAtom(instr->op.el1);
        fprintf(fp,"\n  syscall");
    }

    //printInstrAux(instr);
    fprintf(fp,"\n");
}

void printInstr3(Instr* instr) {
    switch(instr->op.operator) {
    case I_PLUS:
        printAtom(instr->op.el1);fprintf(fp," := ");
        printAtom(instr->op.el2);fprintf(fp," PLUS ");
        printAtom(instr->op.el3);fprintf(fp,";\n");
        break;
    case I_MINUS:
        printAtom(instr->op.el1);fprintf(fp," := ");
        printAtom(instr->op.el2);fprintf(fp," MINUS ");
        printAtom(instr->op.el3);fprintf(fp,";\n");
        break;
    case(I_MULT):
        printAtom(instr->op.el1);fprintf(fp," := ");
        printAtom(instr->op.el2);fprintf(fp," MULT ");
        printAtom(instr->op.el3);fprintf(fp,";\n");
        break;
    case(I_DIV):
        printAtom(instr->op.el1);fprintf(fp," := ");
        printAtom(instr->op.el2);fprintf(fp," DIV ");
        printAtom(instr->op.el3);fprintf(fp,";\n");
        break;
    case(I_ATRIB):
        printAtom(instr->op.el1);fprintf(fp," := ");
        printAtom(instr->op.el2);fprintf(fp,";\n");
        break;
    case(I_LABEL):
        fprintf(fp,"LAB ");printAtom(instr->op.el1);fprintf(fp,"\n");
        break;
    case(I_GOTO):
        fprintf(fp,"GOTO ");printAtom(instr->op.el1);fprintf(fp,"\n");
        break;
    case(I_IFE):
        fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," EQUAL ");printAtom(instr->op.el2);
        fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFDIF):
    	fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," DIF ");printAtom(instr->op.el2);
        fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFG):
    	fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," GT ");printAtom(instr->op.el2);
        fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFL):
    	fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," LESS ");printAtom(instr->op.el2);
        fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFGE):
    	fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," GE ");printAtom(instr->op.el2);
        fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFLE):
    	fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," LE ");printAtom(instr->op.el2);
        fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
	case I_PRINT: 
    	fprintf(fp,"PRINT ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
	case I_SCAN:
    	fprintf(fp,"READ ");printAtom(instr->op.el2);fprintf(fp," ");
        printAtom(instr->op.el1);fprintf(fp,"\n");
        break;
    }
}

void printDataCmd(Instr* instr){
    fprintf(fp,"  %s: ",instr->op.el1->u.name);
    if(instr->op.el2->u.name[0]=='\"' || instr->op.el2->u.name[0]=='\''){
        fprintf(fp,".asciiz ");
    }
    fprintf(fp,"%s\n",instr->op.el2->u.name);
}
void printData() {
    while(data!=NULL){
        printDataCmd(data->instr);
        data=data->next;
    }
}

void printAtom(Atom* ex) {
    switch(ex->kind) {
    case(A_STRING):
        fprintf(fp,"%s", ex->u.name);
        break;
    case(A_INT):
        fprintf(fp,"%d", ex->u.value);
        break;
    case(A_EMPTY):
        fprintf(fp," ");
        break;
    /*case(A_LABEL):
    	fprintf(fp,"%s",ex->)
    	break;*/
    }
}


void printMIPS(InstrList* list) {
    printInstrMIPS(list->instr);
    if(list->next != NULL)printMIPS(list->next);
    else fprintf(fp,"  li $v0, 10\n  syscall\n");
}

void print3Adr(InstrList* list) {
    printInstr3(list->instr);
    if(list->next != NULL)print3Adr(list->next);
    else fprintf(fp,"EXIT\n");
}
void printListMips(InstrList* list) {

    char fileName[25] = "exampleMips.txt";
    fp = fopen(fileName, "w");
    if ( fp == NULL ) {
        printf("Impossivel criar arquivo Mips\n");
        return;
    }
    fprintf(fp, ".data\n");
    printData();
    fprintf(fp, ".text\n");
    fprintf(fp, "main:\n");

    printMIPS(list);
    fclose(fp);
}

void printList3Adr(InstrList* list) {

    char fileName[25] = "example3Adr.txt";
    fp = fopen(fileName, "w");
    if ( fp == NULL ) {
        printf("Impossivel criar arquivo 3 Addresses\n");
        return;
    }
    //hash
    //fprintf(fp, ".data\n");
    print3Adr(list);
    fclose(fp);
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
        //printTemp(code);
        //fprintf(fp,"\n\n");
        //print3Adr(code);
        //fprintf(fp,"\n\n");
        /*if(data!=NULL){
            fprintf(fp,".data:\n");
            printData();
        }
        fprintf(fp,".text:\n");*/
        //printMIPS(code);
        printList3Adr(code);
        printListMips(code);
    }
    return 0;
}