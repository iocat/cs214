CC = gcc
TARGET = sl
CFLAGS = -g -Wall
SLLIB = libsl.a
SLOBJECT = sorted-list.o
all: libsl.a main.c
	$(CC) $(CFLAGS) main.c -L. -lsl -o $(TARGET)

$(SLLIB): $(SLOBJECT)
	$(AR) rcs $@ $<

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) sorted-list.c -c -o $@

clean: 
	rm -f *.o
	rm  $(SLLIB)
	rm $(TARGET)
