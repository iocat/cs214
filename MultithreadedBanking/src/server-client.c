#define SESSION_CLIENT_SHARE_PREFIX "/tmp/.session_client"
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void init_client_shared_mem(session_client_t* share){

}

session_client_t* set_up_client_shared_mem(int* fd,int client_no){
    client_session_t* mapped_data;
    char file_name[100];
    sprintf(file_name,"%s_%d%s",SESSION_CLIENT_SHARE_PREFIX,".bin",client_no);
    *fd = open(file_name,O_CREAT | O_TRUNC );
    if (*fd < 0){
        perror("Cannot open file for sharing memory between the session and client.");
        return NULL;
    }
    mapped_data =(session_client_t*) mmap(NULL,sizeof(session_client_t),
            PROT_READ | PROT_WRITE,*fd,0);
    if(mapped_data == MAP_FAILED){
        perror("Cannot map the data to the file.");
        return NULL;
    }else{
        init_client_shared_mem(mapped_data);
        return mapped_data;
    }
}

void release_client_shared_mem(session_client_t* share,int fd){
    if(munmap(share,sizeof(server_session_t))!=0){
        perror("Cannot unmap the share data between session and the client.");
    }
    close(fd);
}

void client(session_client_t* ses_cli){


}
