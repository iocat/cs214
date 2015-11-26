#ifndef CLIENT_RESPONSE_H
#define CLIENT_RESPONSE_H
typedef struct response_arg_t{
   int client_socket_fd; 

}response_arg_t;

void* response_subroutine(void* arg);
#endif
