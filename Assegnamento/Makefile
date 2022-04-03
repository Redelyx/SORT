all: clean server hub sensor actuator

server: server.o list.o
	gcc -g -Wall -o $@ $^

hub: hub.o list.o
	gcc -g -Wall -o $@ $^

sensor: sensor.o list.o
	gcc -g -Wall -o $@ $^ -lm

actuator: actuator.o list.o
	gcc -g -Wall -o $@ $^

clean:
	rm -f server hub sensor actuator *.o *~

