#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "code.h"
#include "ast.h"
#include "compiler.h"

int counter = 0;
int counter1 = 0;
InstrList* data;
char temp[10];
FILE* fp;

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
            if(isdigit(cmd->type.print.str[0]))
                return mk_instr_list(mk_instr(I_PRINT,atom_variable(cmd->type.print.str),atom_integer(1),atom_variable(cmd->type.print.str),atom_empty()),NULL);
            
            if(cmd->type.print.str[0]=='\'' || cmd->type.print.str[0]=='\"'){
                char* temp=newString();
                l1=mk_instr_list(mk_instr(I_PRINT,atom_variable(temp),atom_variable(cmd->type.print.str),atom_empty(),atom_empty()),NULL);
                data=append(data,l1);
                return mk_instr_list(mk_instr(I_PRINT,atom_variable(temp),atom_integer(4),atom_variable(cmd->type.print.str),atom_empty()),NULL);
            }
            return mk_instr_list(mk_instr(I_PRINT,atom_variable(cmd->type.print.str),atom_integer(4),atom_variable(cmd->type.print.str),atom_empty()),NULL);
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
    }
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
        fprintf(fp," THEN ");printAtom(instr->op.el4);fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFDIF):
        fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," DIF ");printAtom(instr->op.el2);
        fprintf(fp," THEN ");printAtom(instr->op.el4);fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFG):
        fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," GT ");printAtom(instr->op.el2);
        fprintf(fp," THEN ");printAtom(instr->op.el4);fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFL):
        fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," LESS ");printAtom(instr->op.el2);
        fprintf(fp," THEN ");printAtom(instr->op.el4);fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFGE):
        fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," GE ");printAtom(instr->op.el2);
        fprintf(fp," THEN ");printAtom(instr->op.el4);fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
        break;
    case(I_IFLE):
        fprintf(fp,"IF ");printAtom(instr->op.el1);
        fprintf(fp," LE ");printAtom(instr->op.el2);
        fprintf(fp," THEN ");printAtom(instr->op.el4);fprintf(fp," ELSE ");printAtom(instr->op.el3);fprintf(fp,"\n");
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

void print3Adr(InstrList* list) {
    printInstr3(list->instr);
    if(list->next != NULL)
        print3Adr(list->next);
    else
        fprintf(fp,"EXIT\n");
}

void printList3Adr(InstrList* list) {
    char fileName[25] = "example3Adr.txt";
    fp = fopen(fileName, "w");
    if(fp == NULL) {
        printf("Impossivel criar arquivo 3 Addresses\n");
        return;
    }
    print3Adr(list);
    fclose(fp);
}

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
        fprintf(fp,"  la $a0, ");printAtom(instr->op.el1);
        fprintf(fp,"\n  syscall");
        break;
    case(I_SCAN):
        fprintf(fp,"  li $v0, 5\n");
        fprintf(fp,"  la $a0, ");printAtom(instr->op.el1);
        fprintf(fp,"\n  syscall");
    }
    fprintf(fp,"\n");
}

void printMIPS(InstrList* list) {
    printInstrMIPS(list->instr);
    if(list->next != NULL)printMIPS(list->next);
    else fprintf(fp,"  li $v0, 10\n  syscall\n");
}

void printListMips(InstrList* list) {
    char fileName[25] = "exampleMips.txt";
    fp = fopen(fileName, "w");
    if(fp == NULL) {
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
        InstrList* code = CompileCmdList(root);
        printList3Adr(code);
        printListMips(code);
    }
    return 0;
}