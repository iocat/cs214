CFLAGS = -Wall -g
CC = gcc

all: libheap.a main.c
	$(CC) $(CFLAGS) main.c -L. -lheap -o test

libheap.a: heap.o
	$(AR) crs $@ $< 

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c $<
 
clean:
	rm *.a
	rm *.o
