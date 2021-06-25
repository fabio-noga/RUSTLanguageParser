# RUST Language Parser

Implementation of a parser for a subset of the Rust language.

> NOTE: This is an undergraduate curricular unit project (3rd Year).

This representation of the language has:

- a single function: fn main () {...}

- Integers and Variables

- I/O functions: Read variables (read_line()) and Print variables or Strings

- Type Keyword to assignments: let 

- Arithmetic expressions: +, -, *, /, %

- Boolean expressions: ==, !=, <, >, <=, >=, &&, ||

- If and If else

- While Cycles

- Colons and Semicolons

The parser then parses the program for the given RUST code, and outputs a representation of the abstract tree (union in C), a file in 3 address code and a file in a simplistic MIPS representation.

## To execute

```
$  make
$  ./compiler [exemplo.txt]
```
