#include "server-session.h"
#include "server-client.h"
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
void * session_subroutine(void* arg){
    session_thread_info_t* session_thread_info = (session_thread_info_t*) arg;
    pthread_t client_threads[MAX_CLIENT]; 
    memset(client_threads,0,MAX_CLIENT*sizeof(pthread_t));

    client_thread_info_t client_thread_args[MAX_CLIENT];
    memset(client_thread_args, 0 , MAX_CLIENT*sizeof(client_thread_info_t));

    struct sockaddr_in client_addresses[MAX_CLIENT];
    memset(client_addresses,0, MAX_CLIENT*sizeof(struct sockaddr_in));

    int  i = 0 ;
    /* thread_check_mutex locks to check whether the thead is NULL or not   */
    pthread_mutex_t thread_check_mutex;
    pthread_mutex_init(&thread_check_mutex,NULL);
    socklen_t client_address_len;
    int client_socket_fd;
#define socket_fd session_thread_info->server_socket_fd     
    while(1){
        for( i = 0 ; i < MAX_CLIENT; i ++){
            pthread_mutex_lock(&thread_check_mutex);
            if(client_threads[i] == 0){
                pthread_mutex_unlock(&thread_check_mutex);
                break;
            }
            pthread_mutex_unlock(&thread_check_mutex);
        }
        if(i == MAX_CLIENT){
            pthread_mutex_unlock(&thread_check_mutex);
            client_socket_fd = accept(socket_fd, NULL,NULL);
            
            //Tell client to reconnect later
        }else if(( client_socket_fd = accept(socket_fd,
                            (struct sockaddr*) &client_addresses[i],
                            &client_address_len)) < 0){
             perror("Error on accepting client.");
        }else{
            printf("New client is connected on socket (id = %d)\n",client_socket_fd);
            client_thread_args[i].client_socket_fd = client_socket_fd;
            client_thread_args[i].client_addr_ptr = &client_addresses[i];
            client_thread_args[i].client_addr_len = client_address_len;
            client_thread_args[i].accounts = 
                session_thread_info->accounts;
            client_thread_args[i].client_account = NULL;
            client_thread_args[i].accounts_no_ptr =
                session_thread_info->accounts_no_ptr;
            client_thread_args[i].accounts_max = 
                session_thread_info-> accounts_max;
            /* Let the client thread free itself */
            client_thread_args[i].this_thread_ptr = &client_threads[i];
            client_thread_args[i].thread_check_mutex_ptr = 
                &thread_check_mutex;
            client_thread_args[i].new_account_lock_mutex_ptr = 
                session_thread_info->new_account_lock_mutex_ptr;
            pthread_create(&client_threads[i],NULL, client_subroutine,
                    (void*) &client_thread_args[i]);
        }
        
    }

#undef socket_fd
    pthread_mutex_destroy(&thread_check_mutex);
    pthread_exit(NULL);
}
