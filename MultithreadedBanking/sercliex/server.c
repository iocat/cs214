#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#define CLIENT_LIMIT 5
#define IP_ADDRESS "172.31.243.26"
#define LOCAL_HOST "127.0.0.1"
typedef struct thread_info_t{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
} thread_info_t;

void* client_handler( void* client){
    // Cast passing parameter as client data
    thread_info_t* client_data = (thread_info_t*) client;
    char ch = 'A';
#define clientfd client_data->client_fd
    while (1){
        read( clientfd, &ch,1);
        if(ch=='0'){
            close(clientfd);
            pthread_exit(NULL);
        }
        printf("Client ( fd = %d ) gave %c. Now reply to the client.\n",
                clientfd, ch); 
        unsigned long message_size = strlen("ASCII CODE: ")+log10((int)ch)+1;
        char* message_to_client = malloc(message_size+1);
        sprintf( message_to_client,"ASCII CODE: %d\n",(int)ch);
        write( clientfd, message_to_client, message_size + 1);
        free(message_to_client);
    }
    // Exit without returning a void* signal
#undef clientfd 

    pthread_exit(NULL);
}

int main(){
    pthread_t threads[CLIENT_LIMIT];
    thread_info_t thread_info[CLIENT_LIMIT];
    int server_fd;
    int result;
    // Create a socket
    // AF_NET: Internet domain
    // SOCK_STREAM : connection type
    server_fd = socket( AF_INET, SOCK_STREAM, 0);
    // Create address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS) ;
    server_addr.sin_port = htons(9734);
    int server_add_len = sizeof(server_addr);
    // Bind the socket to the address
    result = bind( server_fd, (struct sockaddr*) &server_addr, server_add_len);
    if( result !=0){
        perror("Cannot bind the socket with the requested address");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    // Listen to the clients with a size limited client queue
    result = listen(server_fd,CLIENT_LIMIT);
    if(result !=0){
        perror("Cannot make the socket listen to the client requests");
        exit(EXIT_FAILURE);
    }

    int client_count = -1 ;
    // the main thread listen to client requests
    while (1){
        client_count ++;
        // Accept is blocked if no connection is detected or amount of clients
        // reached limit
        thread_info[client_count].client_fd = accept(server_fd, 
                (struct sockaddr*)&(thread_info[client_count].client_addr),
                &thread_info[client_count].client_addr_len);
        if(thread_info[client_count].client_fd < 0){
            perror("Cannot accept the client connection!");
            exit(EXIT_FAILURE);
        }
        // Create a new thread
        result = pthread_create(&threads[client_count],NULL, client_handler, 
               (void*) &thread_info[client_count]);
        if(result !=0){
            perror("Cannot create a new thread to serve a client");
            exit(EXIT_FAILURE);
        }
    }
    // Collect all the stopped thread here 
}
