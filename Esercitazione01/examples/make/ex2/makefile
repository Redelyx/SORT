esercizio: main.o multiply.o sum.o
	gcc -o esercizio main.o multiply.o sum.o
main.o: main.c multiply.h sum.h
	gcc -c main.c
multiply.o: multiply.c multiply.h
	gcc -c multiply.c
sum.o: sum.c sum.h
	gcc -c sum.c
clean:
	rm *.o esercizio
