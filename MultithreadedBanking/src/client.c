#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "client-command.h"
#include "client-response.h"
#include <pthread.h>
#define SERVER_PORT "9734"
#define WAIT_SERVER_TIME 3
int main(int argc, char* argv[]){
    if(argc<2){
        printf("usage: %s MACHINE_NAME\n",argv[0]); 
        exit(EXIT_SUCCESS);
    }else{
        int client_socket_fd = 0 ;
        struct addrinfo hints,*results;
        pthread_t command_thread;
        pthread_t response_thread;
        command_arg_t command_arg;
        response_arg_t response_arg;
        int response_join;
        /* Get the address info of the server to be connected */
        memset(&hints,0,sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if( getaddrinfo(argv[1],SERVER_PORT,&hints,&results) !=0){
            printf("Cannot recognize the given server name");
            exit(EXIT_FAILURE);
        }
        /* Create the client socket */
        if((client_socket_fd = socket( AF_INET, SOCK_STREAM, 0))<0){
            perror("Cannot create the client socket.");
            exit(EXIT_FAILURE);
        }
        if(results->ai_addr == NULL){
            perror("Cannot find the given server name.");
            exit(EXIT_FAILURE);
        }
        /* Set up the connection */
        while(connect(client_socket_fd,results->ai_addr, 
                    results->ai_addrlen) < 0 ){
            printf("Cannot connect to the server. Retrying...\n");
            sleep(WAIT_SERVER_TIME);
            memset(&hints,0,sizeof hints);
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            freeaddrinfo(results);
            if( getaddrinfo(argv[1],SERVER_PORT,&hints,&results) !=0){
                printf("Cannot recognize the given server name");
                exit(EXIT_FAILURE);
            }
        }
        printf("Successfully connected to the server.\n");
        
        command_arg.client_socket_fd = client_socket_fd;
        pthread_create(&command_thread,NULL,command_subroutine,
                (void*) &command_arg);
        response_arg.client_socket_fd = client_socket_fd;
        pthread_create(&response_thread,NULL,response_subroutine,
                (void*) &response_arg);

        
        if(pthread_join(response_thread,(void**)&response_join)!=0){
            perror("Error stoping the response thread");
            freeaddrinfo(results);
            exit(EXIT_FAILURE);
        }else{
            freeaddrinfo(results);
            exit(EXIT_SUCCESS);
        }
        exit(EXIT_SUCCESS);
    }
}
