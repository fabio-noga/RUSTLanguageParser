#include <stdio.h>
#include "parser.h"
#include "printAbsTree.h"

int start(int argc, char** argv) {
  --argc;
  ++argv;

  if(argc != 0) {
    yyin = fopen(*argv, "r");

    if (!yyin) { //  yyin = stdin
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  }

  if (yyparse() == 0) {
    printf("fn main()\n");
    printExpr(root,1);
  }  
}

int main(int argc, char** argv) {
  start(argc, argv);
  return 0;
}