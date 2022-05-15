CFLAGS=-Wall -g

all: libedsu test

libedsu:
	$(MAKE) -C ../libedsu

test: test.o libedsu.so
	$(CC) -o $@ $< ./libedsu.so

test.o: edsu.h

clean:
	rm -f *.o test


