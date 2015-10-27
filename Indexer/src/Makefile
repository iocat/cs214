CC = gcc
CFLAGS = -g -Wall

# User decide whether to use enhanced malloc or not
# OPTIONS Could be 
# -MALLOC: to allow using enhanced malloc library
OPTIONS =

# SortedList Library
SLLIB = ./SortedList/libsl.a
# Enhanced Malloc and Free Library
MLLIB = ./EnhancedMalloc/libheap.a
# Tokenizer Library
TKLIB = ./Tokenizer/libtk.a
TKDIR = ./Tokenizer

LIB = $(TKLIB) $(MLLIB) $(SLLIB)

TARGET = index 
OBJECTS = index.o
HEADERS = $(wildcard ./**/*.h)

all: $(OBJECTS)  $(LIB)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIB) 

$(TKLIB):
	$(MAKE) -C $(TKDIR)

%.o : %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $< $(OPTIONS)

clean: 
	rm -f *.o 
	rm -f $(TARGET)
	$(MAKE) clean -C $(TKDIR)
