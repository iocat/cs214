#ifndef HASHTABLE_H
#define HASHTABLE_H
#endif

/*
 * A linked list struct
 */
typedef struct HashTableT HashTableT;

/*
 * HTCreate() function creates and returns a HashTable as a array of ListT* pointer.
 * The functions takes in the size of the hash table
 */
HashTableT* HTCreate(int );
/*
 * hash() takes in a key as a string and the size of the hashtable, return the index number
 * where the the key will or can be located
 */
int hash(char*,int);
/*
 * addToHashTable adds a new element into the hash table
 */
void addToHashTable(HashTableT* ,char* , char*);

/*
 * find searches for a value corresponding to the key string and returns it
 */
char* find(HashTableT* , char* );

/*
 * HTDestroy will destroy the hash table and free all allocated memory associated with it
 */
void HTDestroy(HashTableT* );

