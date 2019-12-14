#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "ast.h"

char temp[10];
FILE *fp;


InstrList* compileCmd(Cmd* cmd){
	switch(cmd->kind)
	{
		case C_ASSIGN:
		printf("Atrib\n");
			return compileExpr((cmd->type.assign.aexpr)->expr,(cmd->type.assign.aexpr)->var);
			//return code;


		/*case WHILE:
			inicio = newLabel();
			l_true = newLabel();
			l_false = newLabel();
			cond = compileBool(c->cond, l_true, l_false);
			body = compileCmd(c->cmd);
			code = [LABEL inicio] ++ cond ++ [LABEL l_true] ++ body ++ [GOTO inicio, LABEL l_false]
	*/
	}
}




InstrList* CompileCmdList(CmdList* CmdList) {
	
	if(CmdList!=NULL){
		InstrList* list=compileCmd(CmdList->cmd);
		return append(list,CompileCmdList(CmdList->next));
	}
	return NULL;
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

void printInstrList(InstrList* list) {
	printInstr(list->instr);
	if(list->next != NULL) {
		printInstrList(list->next);
	}
}

void printListMips(CmdList* list, int tabs){

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
void printTemp(InstrList* list){
	printInstr(list->instr);
	printTemp(list->next);
}


int main(int argc, char** argv) {
	--argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  }
  if (yyparse() == 0) {
    //InstrList* result = compileExpr(root);
    printf(".text:\n");
    CompileCmdList(root);
    //printTemp(CompileCmdList(root));
    //printListMips(compileExpr);
  }
  return 0;
}