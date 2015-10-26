CC = gcc 
CFLAGS = -g -Wall
EXE = tokenizer
all: tokenizer.c hashtable.o
	$(CC) $(CFLAGS) $? -o $(EXE)

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) $< -c 

tokenizer: tokenizer.c hashtable.o
	$(CC) $(CFLAGS) $? -o $(EXE)

clean: 
	rm *.o
	rm -f $(EXE)
