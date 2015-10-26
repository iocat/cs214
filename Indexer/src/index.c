#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "SortedList/sorted-list.h"

#ifdef MALLOC
    #include "EnhancedMalloc/heap.h"
#endif


void usage(){
    printf("usage: index [indexfile files]");
}

int main(int argc , char* argv[]){
    if( argc == 1){
        usage();
        int* x = malloc(100);
        free(x);
        free(x);
    }else{
        
    } 
    return EXIT_SUCCESS;
}
