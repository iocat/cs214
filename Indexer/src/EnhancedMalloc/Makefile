CFLAGS = -Wall -g
CC = gcc
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
LIB = libheap.a

all: $(LIB)

$(LIB): $(subst lib,,$(subst .a,.o,$(LIB)))
	ar rcs $@ $<
	

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c 
 
clean:
	rm -f *.o
	rm -f *.a
	rm -f $(TARGET)
