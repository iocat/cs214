/*
 *  server-client.h contains client code
 */
#ifndef _SERVER_CLIENT_H_
#define _SERVER_CLIENT_H_
#include "account.h"
// An abstrct struct for sharing data between the session process and
// the client process 
typedef struct client_t{
    account_t* accounts;

    int* accounts_no_ptr;

    // A pointer to the new account lock mutex
    pthread_mutex_t* new_account_lock_mutex_ptr; 
} client_t;

// The client code that runs on another process
void client(client_t* cli_data,int client_fd);

#endif
