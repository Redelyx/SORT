# Our first Makefile


main: main.o multiply.o 
	gcc -o main main.o multiply.o 

main.o: main.c multiply.h
	gcc -c main.c

multiply.o: multiply.c multiply.h
	gcc -c multiply.c
	
