#ifndef HEAP_H
#define HEAP_H

#ifndef NULL
#define NULL 0
#endif

#define malloc(x) customMalloc(x,__FILE__,__LINE__);
#define free(x) customFree(x,__FILE__,__LINE__);

/*
 *  My malloc implementation
 */
void* customMalloc(unsigned , char*, int);
/*
 *  My free implementation
 */ 
void customFree(void*, char*, int);

#endif
