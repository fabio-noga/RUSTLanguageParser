============================================================
Geração de código
============================================================

O objectivo desta aula é implementar um gerador de código para expressões aritméticas.
O input é uma estrutura Expr do primeiro trabalho prático, adaptado de forma a lidar só com expressões aritméticas ( a variável root será de tipo Expr)

============================================================

1. Baseado no primeiro trabalho crie um novo projeto para construir a árvore abstracta só de expressões aritméticas sem variáveis.

============================================================

2. 
a) Defina ficheiros code.h e code.c para implementar uma estrutura (struct Instr) e respectivos construtores de um código intermédio de três endereços definido por: 

instruction -> VAR := atom | VAR := atom binop atom 

atom -> VAR
atom -> NUMBER

binop -> PLUS | MINUS | DIV | MULT

b) Defina uma estrutura para listas de instruções, InstrList (lista de apontadores para structs _Instr), respectivos construtores e funções de acesso getFirst e nextInstrs, e uma função append para concatenação de listas de instruções.

c) Defina uma função printInstr para imprimir uma instrução.

d) Defina uma função printListIntrs para imprimir uma lista de instruções.

============================================================

3. Defina em C um compilador da árvore abstracta para expressões (struct _Expr) para uma lista de instruções (lista de struct _Instr).

Uma sugestão de protótipo para a função que compila expressões é:

InstrList compileExp(Expr e, char *r); onde r é o registo onde está o valor da availação da expressão na lista de instruções InstrList. Por exemplo:

O resultado da compilação da expressão: x - 2*5 é a lista de instruções:

t1 := x;
t2 := 2;
t3 := 5;
t4 := t1*t2;
t5 := t1 - t4;

e neste caso o registo r é igual a t5.

============================================================

4. Defina em C uma função printMIPS que imprime num ficheiro o código MIPS correspondente à lista de instruções gerada pelo compilador.

Referência para a pergunta 4: https://www2.cs.arizona.edu/~debray/Teaching/CSc453/DOCS/3addr2spim.pdf

=============================================================

5. Repita todas as perguntas anteriores para comandos. Neste caso o código intermédio é definido por:

instruction -> VAR := atom 
instruction -> VAR := atom binop atom
instruction -> GOTO label
instruction -> IF VAR relop atom THEN label ELSE label
instruction -> LAB label

atom -> VAR
atom -> NUMBER

label -> LAB_ID

binop -> PLUS | MINUS | DIV | MULT
relop -> EQUAL | DIF | LESS | GT | LE | GE

Para compilar comandos é necessário ter outra função de compilação de expressões booleanas com protótipo:

InstrList compileBool(ExprBool e; char *labelTrue, char *labelFalse); 

onde labelTrue e labelFalse são as labels para onde a execução do programa em código intermédio deverá ir conforme a compilação da expressão booleana seja respectivamente True ou False. 

A função de compilação de comandos (que chama as função de compilação de expressões aritméticas e booleanas) terá protótipo:

InstrList compileCmd(Cmd c);

