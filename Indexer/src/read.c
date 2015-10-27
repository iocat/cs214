#include "read.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int fd_len(int fd){
    int current_pos = lseek(fd,0,SEEK_CUR);
    int length = lseek(fd,0,SEEK_END);
    lseek(fd,current_pos,SEEK_SET);
    return length;
}
char* read_file(char* path){
    int fd = open(path,O_RDONLY);
    int length=fd_len(fd);
    char* bf;
    if(fd == -1){
        fprintf(stderr,"Cannot open file %s",path);
        exit(EXIT_FAILURE);
    }else{
        lseek(fd,0,SEEK_SET);
        bf = malloc( length+1);
        read(fd,bf,length);
        bf[length] = '\0';      
    }   
    return bf;
}


