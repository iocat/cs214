#include "client-response.h"
#include "account.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void* response_subroutine(void* arg){
    response_arg_t* response_arg = (response_arg_t* ) arg;
    int client_socket_fd = response_arg->client_socket_fd;
    struct timeval timeout = {3000,0};
    fd_set read_fd_set;
    FD_ZERO(&read_fd_set);
    FD_SET(client_socket_fd,&read_fd_set);
    int active_sockets;
    int byte_read;
    response_t response;
    while((active_sockets = select(client_socket_fd+1,&read_fd_set,NULL,NULL,
                    &timeout))>0){
        byte_read = read(client_socket_fd,(void*)&response,sizeof(response_t));
        // Connection is closed
        if(byte_read==0){
            printf("Connection is closed unexpectedly.");
            close(client_socket_fd);
            pthread_exit((void*)EXIT_FAILURE);
        }
        if(response.code !=SUCCESS)
            printf("Error: ");
        printf("Bank: %s\n",response.message);
    }
    // Timeout stop listening to the server. 
    // The account is no longer in session
    if(active_sockets ==0){
        close(client_socket_fd);
        printf("Connection time out. Stop listening to the server.");
        pthread_exit((void*)EXIT_FAILURE);
    }
    pthread_exit((void*)EXIT_FAILURE);
}
