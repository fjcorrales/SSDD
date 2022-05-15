CFLAGS=-Wall -g -fPIC

libedsu.so: edsu.o comun.o
	$(CC) $(CFLAGS) -shared -o $@ $^

edsu.o: comun.h  edsu.h

clean:
	rm -f *.o *.so


