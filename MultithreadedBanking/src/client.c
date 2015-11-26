#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define SERVER_PORT "9734"

int main(int argc, char* argv[]){
    if(argc<2){
        printf("usage: %s MACHINE_NAME\n",argv[0]); 
        exit(EXIT_SUCCESS);
    }else{
        int client_socket_fd = 0 ;
        struct addrinfo hints,*results;


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
        while(connect(client_socket_fd, results->ai_addr, 
                    results->ai_addrlen) < 0 ){
            perror("Cannot connect to the server. Retrying...");
        }
        printf("Connected successfully.\n");
        while(1){
            sleep(100);

        } 
        freeaddrinfo(results);
        exit(EXIT_SUCCESS);
    }
}
