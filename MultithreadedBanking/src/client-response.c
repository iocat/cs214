#include "client-response.h"
#include "account.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
void* response_subroutine(void* arg){
    response_arg_t* response_arg = (response_arg_t* ) arg;
    int client_socket_fd = response_arg->client_socket_fd;
    struct timeval timeout = {5000,0};

    fd_set read_fd_set;
    FD_ZERO(&read_fd_set);
    FD_SET( client_socket_fd ,&read_fd_set);

    int active_sockets;
    int byte_read;
    response_t response;
    while((active_sockets = select(client_socket_fd+1,&read_fd_set,NULL,NULL,
                    &timeout))>0){
        byte_read = read(client_socket_fd,(void*)&response,sizeof(response_t));
        response.code = ntohl(response.code);
        // Connection is closed
        if(byte_read==0){
            printf("Connection from the server is closed unexpectedly.\n");
            pthread_exit((void*)EXIT_SUCCESS);
        }
        // Tell the client this is an error message
        if(response.code !=SUCCESS && response.code != ACCOUNT_EXIT)
            printf("\nError: ");
        printf("%s\n",response.message);
        if (response.code == ACCOUNT_EXIT){
            break;
        } 
    }
    // Timeout stop listening to the server. 
    // The account is no longer in session
    if(active_sockets ==0){
        printf("\nConnection time out. Stop listening to the server.\n");
        pthread_exit((void*)EXIT_FAILURE);
    }
    pthread_exit((void*)EXIT_SUCCESS);
}
