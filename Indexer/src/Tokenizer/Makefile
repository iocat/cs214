CC = gcc
CFLAGS = -g -Wall

TARGET = test
TK = tokenizer
OBJECTS = $(TARGET).o $(TK).o
HEADERS = $(TK).h
TKLIB = libtk.a


all: $(TARGET) $(TKLIB)

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) 

$(TKLIB): $(TK).o
	ar crs $(TKLIB) $<
	rm -f *.o
		
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TKLIB)
	rm -f *.o
	rm -f $(TARGET)
