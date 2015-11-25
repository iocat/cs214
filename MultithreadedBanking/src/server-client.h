/*
 *  server-client.h contains client thread handling function
 */
#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H
#include <pthread.h>
#include "account.h"
/* Thread info struct for client-service threads */
typedef struct client_thread_info_t{
    int client_socket_fd;    
    struct sockaddr_in* client_addr_ptr;
    int client_addr_len; 

    account_t* accounts;
    account_t* client_account;
    int* accounts_no_ptr;
    int accounts_max;
     
    // Point to the current thread data.
    // Set this to 0 before exit
    pthread_t* this_thread_ptr;
    
    /* Mutex lock when free this account thread  from the thread pool */
    pthread_mutex_t* thread_check_mutex_ptr;
    pthread_mutex_t* new_account_lock_mutex_ptr;
} client_thread_info_t;
void* client_subroutine(void*);

#endif
