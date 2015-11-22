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

#define MAX_ACCOUNT 20
#define SERVER_IP_ADDRESS "172.31.243.26"
#define SERVER_PORT 9734
#define WAIT_TIME 4 // seconds

int main(int argc, char* argv[]){
    /* An array of mutex-protected accounts */
    account_t accounts[MAX_ACCOUNT];
    pthread_mutex_t new_account_lock_mutex;
    /* A session acceptor thread
     * This thread accepts multiple connection from new client
     * This thread also take signal from the client_thread that stops */
    pthread_t session_thread;
    session_thread_info_t session_thread_info; 
    void* session_signal = 0;

    /* SERVER SET UP */
    struct sockaddr_in server_address;
    socklen_t server_address_len=0;
    int server_socket_fd ;

    server_socket_fd = socket( AF_INET, SOCK_STREAM, 0);
    // Handle error creating a socket
    if( server_socket_fd < 0){
        perror("Cannot initialize the socket for connection.");
        exit( EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr= inet_addr(SERVER_IP_ADDRESS);
    server_address.sin_port = htons( SERVER_PORT);
    server_address_len = sizeof( server_address);
    // Bind the socket to the given address
    if(bind(server_socket_fd,(struct sockaddr*) &server_address,
                server_address_len) !=0){
        perror("Cannot bind the socket to the given address.");
        exit(EXIT_FAILURE);
    }
    // Allow the socket to listen to the clients
    // The client queue size is MAX_ACCOUNT because we only allow a maximum
    // amount of accounts. So there are maximum 20 clients.
    if(listen(server_socket_fd, MAX_ACCOUNT)!=0){
        perror("Cannot allow the socket to listen to incoming client \
                messages");
        exit(EXIT_FAILURE);
    }
    
    /* CREATE THE SESSION LISTENING THREAD */
    session_thread_info.accounts_max = MAX_ACCOUNT;
    session_thread_info.accounts_no = 0;
    session_thread_info.accounts = accounts;
    session_thread_info.new_account_lock_mutex_ptr = &new_account_lock_mutex;
    session_thread_info.server_socket_fd = server_socket_fd;
    // Init the mutex
    pthread_mutex_init(&new_account_lock_mutex,NULL);
    // Run Session thread
    if(pthread_create(&session_thread, NULL, session_subroutine,(void*) 
           &session_thread_info)!=0){
        perror("Cannot create a session thread.");
        exit(EXIT_FAILURE);      
    }
    /* PRINT BANK ACCOUNT EVERY WAIT_TIME seconds  */
    while(1){
        sleep(WAIT_TIME);   
        pthread_mutex_lock(&new_account_lock_mutex); 
        int temp = 0 ;
        // print all bank account here
        if(session_thread_info.accounts_no == 0){
            printf("Print account: There is no account.");
        }else{
            for(temp = 0; temp <session_thread_info.accounts_no; temp++){
                // Lock the account for reading
                account_t* account = &accounts[temp];
                pthread_mutex_lock(account->account_mutex_ptr);
                printf("Account %d:\n",temp+1);
                printf("Name:\t%s\n",account->name);
                printf("Balance:\t%0.2f\n\n",account->balance);
                if(account->in_session == IN_SESSION){
                    printf("In Session: YES");
                }else if(account->in_session == NOT_IN_SESSION){
                    printf("In Session: NO");
                }
                pthread_mutex_unlock(account->account_mutex_ptr);
            }
        }
        pthread_mutex_unlock(&new_account_lock_mutex);
    }
    /* Main program waits for the session signal to end. */
    if(pthread_join(session_thread,&session_signal)!=0){
        perror("Error ending session service thread");
        exit(EXIT_FAILURE);
    }else{
        // Handle the signal
        printf("Stop server.");
    }
    pthread_mutex_destroy(&new_account_lock_mutex);
    exit(EXIT_SUCCESS);
}
