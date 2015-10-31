CC = gcc
CFLAGS = -g -Wall

# User decide whether to use enhanced malloc or not
# OPTIONS Could be 
# -DMALLOC: to allow using enhanced malloc library
# -DDEBUG: to allow debug mode
OPTIONS =

TARGET = index 
OBJECTS = index.o dirutil.o hashmap.o sorted-list.o tokenizer.o
HEADERS = $(wildcard ./**/*.h)

all: $(TARGET)

debug: 
	$(MAKE) clean
	$(MAKE) OPTIONS=-DDEBUG

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) 

%.o : %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $< $(OPTIONS)

clean: 
	rm -f *.o 
	rm -f $(TARGET)
