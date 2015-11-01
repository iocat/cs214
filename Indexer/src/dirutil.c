#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int fd_len(int fd){
    int current_pos = lseek(fd,0,SEEK_CUR);
    int length = lseek(fd,0,SEEK_END);
    lseek(fd,current_pos,SEEK_SET);
    return length;
}
int is_regular_file(const char* const file_path){
    struct stat path_stat;
    lstat(file_path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

char* regular_file_to_string(const char * const file_path){
    if(is_regular_file(file_path)){
        int fd = open(file_path,O_RDONLY);
        int length=fd_len(fd);
        char* bf;
        if(fd == -1){
            fprintf(stderr,"Cannot open file %s",file_path);
            exit(EXIT_FAILURE);
        }else{
            lseek(fd,0,SEEK_SET);
            bf = malloc( length+1);
            read(fd,bf,length);
            bf[length] = '\0';      
        }   
        return bf;
    }else{
        return NULL;
    }
}
int is_dir(const char* const dir_path){
    struct stat dir_stat;
    stat(dir_path, &dir_stat);
    return S_ISDIR(dir_stat.st_mode);
}
int exists(const char* const path){
    struct stat file_stat;
    return stat(path,&file_stat) == 0 ;
}   
