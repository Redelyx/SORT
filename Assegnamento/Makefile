all: server hub sensore attuatore

server: server.o list.o
	gcc -g -Wall -o $@ $^

hub: hub.o list.o
	gcc -g -Wall -o $@ $^

sensore: sensore.o list.o
	gcc -g -Wall -o $@ $^

attuatore: attuatore.o list.o
	gcc -g -Wall -o $@ $^

clean:
	rm -f server hub sensore attuatore *.o *~

