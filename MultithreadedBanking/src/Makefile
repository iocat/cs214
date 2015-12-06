CC = gcc
CFLAGS = -g -Wall
SERVER = bank
CLIENT = banking
SERVER_SOURCES = server.c server-session.c server-client.c account.c reqres.c
CLIENT_SOURCES = client.c account.c client-command.c client-response.c tokenizer.c reqres.c

all: $(SERVER) $(CLIENT)
	
server: $(SERVER)	
	

client: $(CLIENT)
	

$(SERVER): $(subst .c,.o,$(SERVER_SOURCES))
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(CLIENT): $(subst .c,.o,$(CLIENT_SOURCES))
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
	rm -f $(CLIENT)
	rm -f $(SERVER)
