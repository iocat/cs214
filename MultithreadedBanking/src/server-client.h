/*
 *  server-client.h contains client thread handling function
 */
#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

/* Thread info struct for client-service threads */
typedef struct client_thread_info_t{
    account_t* client_account;
    pthread_mutex_t* new_account_lock_mutex_ptr;
} client_thread_info_t;
void* client_subroutine(void*);

#endif
