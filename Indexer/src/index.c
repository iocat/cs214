#include <stdio.h>
#include <stdlib.h>
#include "dirutil.h"
#include "SortedList/sorted-list.h"
#include "Tokenizer/tokenizer.h"
#ifdef MALLOC
    #include "EnhancedMalloc/heap.h"
#endif

struct Index {
    char* token;
    SortedListPtr records;
};
struct Record {
    char* file;
    int count;
};
int compareIndex(void* index1, void* index2){

}
void destrucIndex(void* index){

}
int compareRecord(void* record1, void* record2 ){

}
void destructRecord(void* record){

}

void usage(){
    printf("usage: index <inverted-index file name> <directory or file name>\n");
}
void make_index(char* file,struct Index* index ){

}

int main(int argc , char* argv[]){
    if( argc == 1){
        usage();
    }else if(argc ==2 ){
        fprintf(stderr,"Error: Not enough arguments\n");
    }else{
        char* index_file = argv[1];
        char* file_dir = argv[2];
        if(is_regular_file(file_dir)){
            /* The given path is a file */
            printf("This is a file");
        } else if(is_dir(file_dir)){
            /* The given path is a directory */
            printf("This is a dir");  
        }else{
            fprintf(stderr,"Error: Invalid file/directory to be indexed");  
        }
    } 
    return EXIT_SUCCESS;
}
