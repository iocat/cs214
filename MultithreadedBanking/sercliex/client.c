#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#define IP_ADDRESS "172.31.243.26"
#define LOCAL_HOST "127.0.0.1"
#define BUFFER_SIZE 1000


struct timeval tv ;


int main(){
 


    struct sockaddr_in server_address;
    int result;
    int server_address_length;
    //==================================================
    // SERVER SETUP
    // Create a socket file descriptor for this client
    int client_fd = socket( AF_INET, SOCK_STREAM, 0);
    if(client_fd <0){
        perror("Cannot create a socket.");
        exit(EXIT_SUCCESS);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr  = inet_addr(IP_ADDRESS);    
    server_address.sin_port = htons(9734);
    server_address_length = sizeof(server_address);
   
    result = connect(client_fd, (struct sockaddr*)&server_address,
            server_address_length);
    if(result==-1){
        perror("Cannot bind the socket with the server address");
        exit(EXIT_FAILURE);
    } 
    //===================================================
    char ch;
    char server_message[BUFFER_SIZE];
    
    // Initialize minimum timeout
    tv.tv_sec = 2;
    tv.tv_usec = 400000;
    
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(client_fd, &readfds);
    while(1){
        printf("Please enter a character\n");
        fscanf(stdin,"%c",&ch);
        while( ch == '\n'){
            fscanf(stdin,"%c",&ch);
        } 
        write(client_fd, &ch, 1);
        // Select waits on the file descriptor until timeout.
        if(select(client_fd+1,&readfds,NULL, NULL,&tv  )==1){
            result = recv(client_fd,server_message, BUFFER_SIZE,0);
            if(result == 0){
                perror("Server closed.");
                close(client_fd);
                exit(EXIT_FAILURE);
            }
        }else{
            printf("Connection timed out.");
            close(client_fd);
            exit(EXIT_FAILURE);
        }
        printf("%s",server_message);

    }      
        


}
