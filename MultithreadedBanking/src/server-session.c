#include "server-session.h"
#include "server-client.h"
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#define SERVER_SESSION_SHARE_FILE "/tmp/.server_session.bin"

// init_session_shared_mem  initilizes data of the shared memory
void init_session_shared_mem(server_session_t* share){
    memset(share->accounts,0,sizeof(share->accounts));
    share->accounts_no = 0;
    pthread_mutex_init(&(share->new_account_lock_mutex),NULL);
}

server_session_t* set_up_session_shared_mem(int* fd){
    server_session_t* mapped_data;
    *fd = open(SERVER_SESSION_SHARE_FILE, O_TRUNC | O_RDWR | O_CREAT,0666);
    if (*fd <0){
        perror("Cannot open file for sharing memory between SERVER and SESSION");
        return NULL;
    }
    ftruncate(*fd,sizeof(server_session_t)+1);
    mapped_data =(server_session_t*) mmap(NULL,sizeof(server_session_t),
            PROT_READ | PROT_WRITE,MAP_FILE | MAP_SHARED,*fd,0);
    if(mapped_data == MAP_FAILED){
        perror("Cannot map the data to the file");
        return NULL;
    }else{
        init_session_shared_mem(mapped_data);
        return mapped_data;
    }
}
void release_session_shared_mem(server_session_t* share,int fd){
    pthread_mutex_destroy(&(share->new_account_lock_mutex));
    if(munmap(share,sizeof(server_session_t))!=0){
        perror("Cannot unmap the share data between server and session.");
    }
    printf("Released shared memory.\n");
    close(fd);
}

typedef struct client_collector_t{
    int client_socket_fd;
    int client_pid;
}client_collector_t;

void* client_collect(void* arg){
    client_collector_t* cc_arg = (client_collector_t*) arg;
    int exit_stt;
    waitpid(cc_arg->client_pid,&exit_stt,WUNTRACED); 
    // Close the client socket
    close(cc_arg->client_socket_fd);
    printf("Connection with client ( fd = %d ) is closed.\n",
            cc_arg->client_socket_fd);
    // Free the allocated argument
    free(arg);
    printf("Collect and destroy child process %d.\n",cc_arg->client_pid);
    pthread_exit(NULL);
}

void session(server_session_t* ser_ses, int server_socket_fd){
    while (ser_ses->stop_signal==0){
        int client_socket_fd;
        struct sockaddr_in client_addr;
        socklen_t sock_len;
        // Accept connection from the client
        if((client_socket_fd=accept(server_socket_fd,(struct sockaddr*)&client_addr,
                        &sock_len))<0){
            perror("Cannot accept one client connection");
        }else{
            int client_pid;
            printf("One client connected ( fd = %d ).\n",client_socket_fd);
            if((client_pid = fork())==0){
                // The child/client process 
                client_t client_data;
                client_data.accounts = ser_ses->accounts;
                client_data.accounts_no_ptr = &(ser_ses->accounts_no);
                client_data.new_account_lock_mutex_ptr = 
                    &(ser_ses->new_account_lock_mutex);
                client(&client_data,client_socket_fd);
            }else{
                printf("Create process %d to handle client %d.\n",client_pid,
                        client_socket_fd);
                pthread_t new_thread;
                // The session process line of execution
                 client_collector_t* cc_arg = (client_collector_t*)
                     malloc(sizeof(client_collector_t));
                 cc_arg->client_socket_fd = client_socket_fd;
                 cc_arg->client_pid = client_pid;
                 // Create client collector thread
                 pthread_create(&new_thread,NULL,client_collect,(void*)cc_arg);
            }
        }
    }
    exit(EXIT_SUCCESS);
}
