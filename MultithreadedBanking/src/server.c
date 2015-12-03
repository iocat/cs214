#include "account.h"
#include "server-session.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <ifaddrs.h>
#define MAX_ACCOUNT 20
#define SERVER_PORT 9734
// seconds
#define WAIT_TIME 20

void print_account(server_session_t* ser_ses,int* server_stop){
    // PRINT BANK ACCOUNT EVERY WAIT_TIME seconds  
    // while the server does not stop
    int time_to_sleep_left;
    while(!(*server_stop)){
        time_to_sleep_left = WAIT_TIME;
        // Damn, you have to sleep with the a amount of time!!
        while((time_to_sleep_left = sleep(time_to_sleep_left))!=0){
            continue;
        }
        pthread_mutex_lock(&(ser_ses->new_account_lock_mutex)); 
        int temp = 0 ;
        // print all bank account here
        if(ser_ses->accounts_no == 0){
            printf("Print account: There is no account.\n");
        }else{
            printf("\n");
            printf("%9s%50s%10s%13s\n","Account","Name","Balance","In Session");
            for(temp = 0; temp <ser_ses->accounts_no; temp++){
                // Lock the account for reading
                account_t* account = &(ser_ses->accounts[temp]);
                pthread_mutex_lock(&(account->account_mutex));
                printf("%9d",temp+1);
                printf("%50s",account->name);
                printf("%10.2f ",account->balance);
                if(account->in_session == IN_SESSION){
                    printf("%13s","YES");
                }else if(account->in_session == NOT_IN_SESSION){
                    printf("%13s","NO");
                }
                pthread_mutex_unlock(&(account->account_mutex));
                printf("\n");
            }
        }
        pthread_mutex_unlock(&ser_ses->new_account_lock_mutex);
    }
}



int main(int argc, char* argv[]){
    /* SERVER SET UP */
    int enable_reuse_socket = 1;
    struct sockaddr_in server_address;
    socklen_t server_address_len=0;
    int server_socket_fd ;
    
    server_socket_fd = socket( AF_INET, SOCK_STREAM, 0);
    // Handle error creating a socket
    if( server_socket_fd < 0){
        perror("Cannot initialize the socket for connection.");
        exit( EXIT_FAILURE);
    }

    // Set up the INET address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr= htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);
    server_address_len = sizeof( server_address);
    // Bind the socket to the given address
    if(setsockopt(server_socket_fd,SOL_SOCKET,SO_REUSEADDR,&enable_reuse_socket
                ,sizeof(int))<0){
        perror("Fail to reuse socket.");
    }
    if(bind(server_socket_fd,(struct sockaddr*) &server_address,
                server_address_len) !=0){
        perror("Cannot bind the socket to the given address.");
        exit(EXIT_FAILURE);
    }
    // Allow the socket to listen to the clients
    if(listen(server_socket_fd, MAX_CLIENT)!=0){
        perror("Cannot allow the socket to listen to incoming client \
                messages");
        exit(EXIT_FAILURE);
    }
   
    //------------------------------------------------------> 
    //PROGRAM STARTS HERE
    // The file descriptor of shared memory file
    int ser_ses_fd;
    // The server session share memory pointer ( to be mapped)
    server_session_t* ser_ses;
    ser_ses = (server_session_t*) set_up_session_shared_mem(&ser_ses_fd);
    if(ser_ses == NULL){
        exit(EXIT_FAILURE);
    }    
    int child_pid;
    int server_stop = 0;
    if((child_pid = fork()) == 0){
        // The child/session process
        session(ser_ses,server_socket_fd); 
    }else{
        int exit_status;
        // The parent process -> print account. and collect the child process
        print_account(ser_ses,&server_stop); 
        // when receives the stop signal, we wait for the session process
        wait(&exit_status);
        release_session_shared_mem(ser_ses,ser_ses_fd);
    } 
    exit(EXIT_SUCCESS);
    // PROGRAM ENDS HERE
    // <------------------------------------------------------------
}
