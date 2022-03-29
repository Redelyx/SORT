all: server casa_e lettore

server:	server.c list.c
	gcc -Wall -o server server.c

casa_e:	casa_e.c list.c
	gcc -Wall -o casa_e casa_e.c
	
lettore: lettore.c list.c
	gcc -Wall -o lettore lettore.c

clean:
	rm -f lettore casa_e server *~

