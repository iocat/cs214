CC = gcc
CFLAGS = -g -Wall

# User decide whether to use enhanced malloc or not
# OPTIONS Could be 
# -DMALLOC: to allow using enhanced malloc library
# -DDEBUG: to allow debug mode
OPTIONS =

# SortedList Library
SLDIR = ./SortedList/
SLLIB = libsl.a
# Enhanced Malloc and Free Library
MLLIB = libheap.a
MLDIR = ./EnhancedMalloc/
# Tokenizer Library
TKLIB =  libtk.a
TKDIR = ./Tokenizer/

LIB = $(TKDIR)$(TKLIB) $(MLDIR)$(MLLIB) $(SLDIR)$(SLLIB)

TARGET = index 
OBJECTS = index.o dirutil.o
HEADERS = $(wildcard ./**/*.h)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(LIB)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIB) 

$(SLDIR)$(SLLIB):
	$(MAKE) -C $(SLDIR) $(SLLIB)

$(MLDIR)$(MLLIB):
	$(MAKE) -C $(MLDIR) $(MLLIB)

$(TKDIR)$(TKLIB):
	$(MAKE) -C $(TKDIR) $(TKLIB)


%.o : %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $< $(OPTIONS)

clean: 
	rm -f *.o 
	rm -f $(TARGET)
	$(MAKE) clean -C $(TKDIR)
	$(MAKE) clean -C $(MLDIR)
	$(MAKE) clean -C $(SLDIR)
