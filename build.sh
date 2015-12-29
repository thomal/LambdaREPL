#/bin/sh

#Build
flex src/patterns
yacc -d src/grammar -o lambda.cpp
gcc -c lex.yy.c -o lex.yy.o -Iinclude
g++ -g lex.yy.o src/*.cpp lambda.cpp -o lambda -Iinclude

#Clean
rm lex.yy.c
rm lex.yy.o
rm lambda.cpp
rm lambda.hpp
