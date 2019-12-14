#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "printAbsTree.h"

void printExpr(Expr* expr, int tabs){
	printTabs(tabs);
	switch(expr->kind) {
		case E_INTEGER:
			printf("%d\n", expr->attr.value);
			break;
		case E_VARIABLE:
			printf("%s\n", expr->attr.variable);
			break;
		case E_OPERATION:
			switch(expr->attr.op.operator) {
				case PLUS:
					printf("+\n");
					break;
				case MINUS:
					printf("-\n");
					break;
				case MULT:
					printf("*\n");
					break;
				case DIV:
					printf("/\n");
					break;
				case MOD:
					putchar('%');
					printf("\n");
					break;
			}
			printExpr(expr->attr.op.left, tabs+1);
			printExpr(expr->attr.op.right, tabs+1);
	}
}

void printBool(BoolExpr* bexpr, int tabs){
	printTabs(tabs);
	switch(bexpr->kind){
		case BOOLINT:
			printf("%d\n", bexpr->attr.value);
			break;
		case EXPR:
			switch(bexpr->attr.boolop.operator){
				case EQ:
					printf("==\n");
					break;
				case DIFF:
					printf("!=\n");
					break;
				case LESS:
					printf("<\n");
					break;
				case GREATER:
					printf(">\n");
					break;
				case LEQ:
					printf("<=\n");
					break;
				case GEQ:
					printf(">=\n");
					break;
			}
		printExpr(bexpr->attr.boolop.left, tabs+1);
		printExpr(bexpr->attr.boolop.right, tabs+1);
	}
}

void printAssign(AssignExpr* aexpr, int tabs) {
	char* var = aexpr->var;
	printTabs(tabs);
	printf("%s\n", strdup(var));
	printExpr(aexpr->expr, tabs);
}

void printCmd(Cmd* cmd, int tabs) {
	if (cmd == 0) {
	    yyerror("Null expression!");
	    return;
	}
	printTabs(tabs);
	switch(cmd->kind) {
		case C_ASSIGN:
			printf("=\n");
			printAssign(cmd->type.assign.aexpr, tabs+1);
			break;
		case C_IF:
			printf("if\n");
			printBool(cmd->type.ifthen.bexpr, tabs+1);
			printList(cmd->type.ifthen.expr, tabs+1);
			break;
		case C_IFELSE:
			printf("if\n");
			printBool(cmd->type.ifelse.bexpr, tabs+1);
			printList(cmd->type.ifelse.expr, tabs+1);
			printTabs(tabs);
			printf("else\n");
			printList(cmd->type.ifelse.eexpr, tabs+1);
			break;
		case C_WHILE:
			printf("while\n");
			printBool(cmd->type.whilethen.bexpr, tabs+1);
			printList(cmd->type.whilethen.expr, tabs+1);
			break;
		case C_PRINT:
			printf("println!\n");
			printTabs(tabs+1);
			printf(" %s \n", cmd->type.print.str);
			break;
		case C_READ:
			printf("read_line &\n");
			printExpr(cmd->type.read.var, tabs+1);
			break;
	}
}

void printList(CmdList* list, int tabs){
	if(list != NULL) {
	    printCmd(list->cmd, tabs);
	    printList(list->next, tabs);
	}
}
void printTabs(int tabs) {
	printf(" ");
	for(int i=1; i<tabs; i++) {
		printf(" │ ");
	}
	printf(" ├ ");
}