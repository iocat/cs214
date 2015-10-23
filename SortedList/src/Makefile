CC = gcc
CFLAGS = -g -Wall
SLLIB = libsl.a
all: libsl.a main.c
	$(CC) $(CFLAGS) main.c -L. -lsl -o sl

libsl.a: sorted-list.o
	$(AR) rcs $@ $<

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) sorted-list.c -c -o $@

clean: 
	rm sorted-list.o 
