CFLAGS=-Wall -g -Iutil

all: libutil broker

libutil:
	$(MAKE) -C util

broker.o: comun.h
comun.o: comun.h

broker: broker.o comun.o libutil.so
	$(CC) -o $@ $< comun.o -lpthread ./libutil.so

clean:
	rm -f *.o broker
