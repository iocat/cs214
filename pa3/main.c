#include <stdio.h>
#include <stdlib.h>
#include "heap.h" 

#define malloc(x) customMalloc(x,__FILE__,__LINE__);
#define free(x) customFree(x,__FILE__,__LINE__);


// Free the memory that is not malloc()
void not_malloced_free(){
    int a =100;
    free(&a);
}

// Free the memory where it is not the beginning of the malloced one
void free_middle(){
    int * x = (int*) malloc(sizeof(int)*100);
    free(x+10);
}

// Redundantly free the memory
void redundantly_free(){
    int *y = (int*) malloc(100);
    free(y);
    free(y);
}

// Valid free
void valid_free(){
    int *z = (int*) malloc(200);
    free(z);
    int *x = z;
    z = (int*) malloc(200);
    free(z);
    if(x==z){
        printf("x = z = %p. New malloced address is the same as the old one.\n",x);
    }
    printf("Perfectly Valid Free().\n");
}

// Insufficient amount of memory error
void insufficient_memory_malloc(){
    int *t = (int*) malloc(5000);
}
int main(int argc, char* argv[]){
    printf("----------------------------------------------------------\n");
    printf("Test 1: Free the memory of a variable located on the stack\n"); 
    not_malloced_free();
    
    printf("----------------------------------------------------------\n");
    printf("Test 2: Free the memory at the address which is not returned by malloc()\n");
    free_middle();

    printf("----------------------------------------------------------\n");
    printf("Test 3: Redundantly free the same address the second time\n");
    redundantly_free();

    printf("----------------------------------------------------------\n");
    printf("Test 4: Reallocate a new memory on the same freed address and free it again\n");
    valid_free();

    printf("----------------------------------------------------------\n");
    printf("Test 5: Request a great amount of memory that the library cannot provide\n");
    insufficient_memory_malloc();    

    return 0;
}
