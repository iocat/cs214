#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#ifdef MALLOC
    #include "EnhancedMalloc/heap.h"
#endif

void usage();
void next();

int main(int argc , char* argv[]){
    printf("%s",bf);
    if( argc == 1){
        usage();
    }else{
        char* index_file = argv[1];
    } 
    return EXIT_SUCCESS;
}
void usage(){
    printf("usage: index <inverted-index file name> <directory or file name>\n");
}
