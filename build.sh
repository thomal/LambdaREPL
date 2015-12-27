#/bin/sh

#Build
flex patterns
yacc -d grammar -o lambda.cpp
gcc -c lex.yy.c -o lex.yy.o
g++ -g lex.yy.o lambda.cpp ast.cpp repl.cpp -o lambda

#Clean
rm lex.yy.c
rm lex.yy.o
rm lambda.cpp
rm lambda.hpp
