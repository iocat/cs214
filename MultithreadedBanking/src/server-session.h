/*
 *  server-session.h contains the session code for the session proccess
 *  where it proactively create new client processes
 *
 */
#ifndef _SERVER_SESSION_H_
#define _SERVER_SESSION_H_
#include "account.h"
#define MAX_CLIENT 1000
// An abstract struct for sharing data between the server process and 
// the session process
typedef struct server_session_t{
    // The account array
    account_t accounts[MAX_ACCOUNT];
    // The current number of accounts
    int accounts_no;
    // The mutex to lock creating a new account
    pthread_mutex_t new_account_lock_mutex;
    // Stop signal 
    int stop_signal;
}server_session_t;

// set_up_session_shared_mem sets up the file containing the shared memory
server_session_t* set_up_session_shared_mem(int* fd);

// release_session_shared_mem releases the mapped memory and close
// the file that stores mapped sharing memory
void release_session_shared_mem(server_session_t* share,int fd);

// session is the session function to be run in the session process
void session(server_session_t* ser_ses, int server_socket_fd);


#endif
