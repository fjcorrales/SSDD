#CC=c99

CFLAGS=-Wall -g -fPIC

all: libutil.so demo_map_key_string demo_map_key_int demo_map_key_struct demo_queue demo_set demo_integral

map.o: map.h
set.o: set.h
queue.o: queue.h

libutil.so: map.o queue.o set.o
	$(CC) $(CFLAGS) -shared -o $@ $^

demo_map_key_string: demo_map_key_string.o libutil.so
	$(CC) -o $@ $< ./libutil.so

demo_map_key_int: demo_map_key_int.o libutil.so
	$(CC) -o $@ $< ./libutil.so

demo_map_key_struct: demo_map_key_struct.o libutil.so
	$(CC) -o $@ $< ./libutil.so

demo_queue: demo_queue.o libutil.so
	$(CC) -o $@ $< ./libutil.so

demo_set: demo_set.o libutil.so
	$(CC) -o $@ $< ./libutil.so

demo_integral: demo_integral.o libutil.so
	$(CC) -o $@ $< ./libutil.so

clean:
	rm -f *.o libutil.so demo_map_key_string demo_queue demo_map_key_int demo_map_key_struct demo_set demo_integral
