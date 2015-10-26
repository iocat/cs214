#ifndef INDEX_H
#define INDEX_H
/*
 * Print usage message.
 */
void usage();

typedef struct{
    char* token;
    SortedListPtr records;
} Index;

typedef struct {
    char* file;
    int count;
} Record;

int compareIndex(void*, void*);
void destrucIndex(void*);
int compareRecord(void*, void*);
void destructRecord(void*);

#endif
