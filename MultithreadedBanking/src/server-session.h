/*
 *  server-session.h contains thread subroutine for handling incoming 
 *  requests from user
 *
 */
#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H
#include "account.h"
#include <pthread.h>
/* A thread info struct for the session thread */
typedef struct session_thread_info_t{
    account_t* accounts;
    int accounts_no;
    int accounts_max;        
    /* Client account protected mutex */
    pthread_mutex_t* new_account_lock_mutex_ptr;
    int server_socket_fd;
} session_thread_info_t;

void* session_subroutine (void*);

#endif
