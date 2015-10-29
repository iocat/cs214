#include "heap.h"
#include <stdio.h>
#define HEAP_SIZE 5000
#define ENTRY_SIZE sizeof( MemEntry )
// boolean data type
typedef enum {false =0, true} bool;

typedef enum{
    HEAP_MALLOC_INSUFFICIENT,
    HEAP_FREE_REDUNDANT,
    HEAP_FREE_NOT_ALLOCATED,
    HEAP_FREE_MIDDLE
} HeapError;
// A "fake" heap
static char mem[HEAP_SIZE] = {'0'};


typedef struct MemEntry MemEntry;
typedef MemEntry* MemEntryPtr;

struct MemEntry{
    // The previous entry
    MemEntryPtr prev;

    // The next entry
    MemEntryPtr next;

    // Whether the chunk of memory is freed or not
    bool isFree;
    
    // fileFree indicates the file containing the free command which frees the entry
    char *fileFree;

    // lineFree indicates the line containing the free command which frees the entry
    int lineFree;
    // Available size for or the size of the data
    // When isFree-d, it stores the available size for allocation
    // Otherwise, size is the size of memory allocated
    unsigned size;
};

// initialized shows whether the head is constructed or not
static bool initialized = false;
// The head pointer
static MemEntryPtr head;

void errorReport( char* file, int caller_line, HeapError error, void* address){
    fprintf(stderr,"%s:%d: error: ",file,caller_line);
    switch(error){
    case HEAP_MALLOC_INSUFFICIENT:
        fprintf(stderr,"Memory is insufficient to malloc().");
        break;
    case HEAP_FREE_REDUNDANT:
        fprintf(stderr,"Memory @%p is already free-d.", address);
        break;
    case HEAP_FREE_NOT_ALLOCATED:
        fprintf(stderr,"Memory @%p is not pre-allocated.",address);
        break;
    case HEAP_FREE_MIDDLE:
        fprintf(stderr,"Memory @%p is not the previously returned address by malloc().",address);
        break;
    }
    printf("\n");
}   

void* customMalloc(unsigned size, char* file, int caller_line){
    if(!initialized){
        // Initialize the head
        head = (MemEntryPtr) mem;
        head->prev = NULL;
        head->next = NULL;
        head->isFree = true;
        head->fileFree = NULL;
        head->lineFree = 0;
        head->size = HEAP_SIZE - ENTRY_SIZE;
        initialized = true;
    }
    MemEntryPtr temp = head;
    
    // While the tracking node is non NULL
    while(temp!=NULL){
        // the node is freed
        if( temp->isFree && temp->size >= size + ENTRY_SIZE ){
            // If find a big-enough chunk and is not freed
            MemEntryPtr newEntry = (MemEntryPtr) ( ((char*)temp) + ENTRY_SIZE + size);
            //Change the node's pointers
            newEntry->prev = temp;
            newEntry->next = temp->next;
            temp->next = newEntry;
            if(newEntry->next!=NULL){
                newEntry->next->prev = newEntry;
            }
            newEntry->isFree = true; 
            //Tweak the current available size of the entry
            newEntry->size = temp->size - size - ENTRY_SIZE;

            temp->isFree = false;
            temp->size = size;
            return (void*) ((char*) temp)+ENTRY_SIZE;
        }
        temp = temp->next;
    }
    // No space to filled in
    errorReport(file,caller_line, HEAP_MALLOC_INSUFFICIENT, 0);
    return NULL; 

}
void customFree(void* data, char* file, int caller_line){
    if(!initialized){
        errorReport(file,caller_line, HEAP_FREE_NOT_ALLOCATED, data);
    }else{
        // tracking node
        MemEntryPtr temp = head;
        while ( temp!=NULL){
            char* offset = ((char*) temp) +ENTRY_SIZE;
            if( offset < (char*) data &&  (char*) data < offset + temp->size){
                // MEMORY IS FREED WHERE IT IS NOT ALLOCATED
                errorReport(file,caller_line,HEAP_FREE_MIDDLE,data);
                return;
            }else if(offset == (char*) data){
                if(temp->isFree==true){
                    // MEMORY WAS FREED - REDUNDANTLY FREE THE MEMORY
                    errorReport(file,caller_line,HEAP_FREE_REDUNDANT,data);
                    fprintf(stderr,"\tDid you free it at line %d in %s ?\n",temp->lineFree,temp->fileFree);
                    return;
                }else{  
                    // FREE THE MEMORY
                    temp->isFree = true;
                    temp->fileFree = file;
                    temp->lineFree = caller_line;
                    // Merge it with previous free chunked
                    if(temp->prev!=NULL && temp->prev->isFree){
                        //Expand the available size of the previous node
                        temp->prev->size+=(temp->size+ENTRY_SIZE) ;
                        // Reassign the node's pointers
                        if(temp->next!=NULL)
                            temp->next->prev= temp->prev;
                        temp->prev->next = temp->next;
                        //Make the tracking node the previous node
                        temp=temp->prev;
                    }
                    if(temp->next!=NULL && temp->next->isFree){
                        temp->size +=  temp->next->size + ENTRY_SIZE;
                        temp->next = temp->next->next;
                        if(temp->next!=NULL){
                            temp->next->prev = temp;
                        }
                    }
                    break;
                }
            }else{
                // Get to the next memory entry
                temp = temp->next;
            }
        }
        // Get to the end of the memory array
        if(temp==NULL)
            errorReport(file,caller_line,HEAP_FREE_NOT_ALLOCATED,data);
    }
    
}
