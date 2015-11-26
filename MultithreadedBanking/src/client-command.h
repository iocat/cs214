#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

typedef struct command_arg_t{
    int client_socket_fd;
}command_arg_t;

void* command_subroutine(void* arg);

#endif
