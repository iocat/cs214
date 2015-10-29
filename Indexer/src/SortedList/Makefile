CC = gcc
CFLAGS = -g -Wall
SLLIB = libsl.a
SLOBJECT = sorted-list.o
all: $(SLLIB)

$(SLLIB): $(SLOBJECT)
	$(AR) rcs $@ $<

%.o:%.c 
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f *.o
	rm  $(SLLIB)
