CC = gcc
CFLAGS = -g -Wall

# User decide whether to use enhanced malloc or not
# OPTIONS Could be 
# -MALLOC: to allow using enhanced malloc library
OPTIONS =

# SortedList Library
SLDIR = ./SortedList/
SLLIB = libsl.a
SLDIRFLAG = -L$(SLDIR)
SLFLAG= -l$(subst lib,,$(subst .a,,$(SLLIB)))
# Enhanced Malloc and Free Library
MLDIR = ./EnhancedMalloc/
MLLIB = libheap.a
MLDIRFLAG = -L$(MLDIR)
MLFLAG = -l$(subst lib,,$(subst .a,,$(MLLIB)))

TARGET = index 
OBJECTS = index.o
HEADERS = $(wildcard ./**/*.h)

all: $(SLDIR)$(SLLIB) $(MLDIR)$(MLLIB) $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(SLDIRFLAG) $(SLFLAG) $(MLDIRFLAG) $(MLFLAG)

%.o : %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $< $(OPTIONS)

clean: 
	rm -f *.o 
	rm -f $(TARGET)

