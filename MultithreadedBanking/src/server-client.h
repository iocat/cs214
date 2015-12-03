/*
 *  server-client.h contains client code
 */
#ifndef _SERVER_CLIENT_H_
#define _SERVER_CLIENT_H_

// An abstrct struct for sharing data between the session process and
// the client process 
typedef struct session_client_t{
    // The socket file descriptor of the client retrieved from accept
    int client_fd;    
    // A pointer to the new account lock mutex
    pthread_mutex_t* new_account_lock_mutex; 
} session_client_t;

// set_up_client_shared_mem sets up the sharing memory between the 
// session and the client processes
//
// NULL is returned if the function fails to create a shared memory
session_client_t* set_up_client_shared_mem(int* fd,int client_no);

// release_client_shared_mem unmaps the shared memory between the session
// and the client processes and close the shared file
void release_client_shared_mem(session_client_t* share,int fd);

// The client code 
void client(session_client_t* ses_cli);

#endif
