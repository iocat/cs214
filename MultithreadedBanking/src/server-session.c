#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#define SERVER_SESSION_SHARE_FILE "/tmp/.server_session.bin"

// init_session_shared_mem  initilizes data of the shared memory
void init_session_shared_mem(server_session_t* share){
    memset(share->accounts,0,sizeof(accounts));
    share->accounts_no = 0;
    pthread_mutex_init(&(share->new_account_lock_mutex),NULL);
}

server_session_t* set_up_session_shared_mem(int* fd){
    server_session_t* mapped_data;
    *fd = open(SERVER_SESSION_SHARE_FILE,O_CREAT | O_TRUNC );
    if (*fd <0){
        perror("Cannot open file for sharing memory between SERVER and SESSION.");
        exit(EXIT_FAILURE);
    }
    mapped_data =(server_session_t*) mmap(NULL,sizeof(server_session_t),
            PROT_READ | PROT_WRITE,*fd,0);
    if(mapped_data == MAP_FAILED){
        perror("Cannot map the data to the file.");
        exit(EXIT_FAILURE);
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
    close(fd);
}

void session(server_session_t* ser_ses, int server_socket_fd){

}
