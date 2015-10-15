#include <stdio.h>
#include <stdlib.h>
#include "heap.h" 

#define malloc(x) customMalloc(x,__FILE__,__LINE__);
#define free(x) customFree(x,__FILE__,__LINE__);


int main(int argc, char* argv[]){
    int x =100;
    free(&x);
    
    char* hoo = (char*) malloc(10);
    free(hoo);
    free(hoo);

    char* charP =(char*) malloc(100);
    free(charP);
    
    charP = (char*)malloc(10);
    free(charP);
    
    return 0;
}
