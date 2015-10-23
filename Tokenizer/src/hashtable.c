#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hashtable.h"

typedef struct ListT ListT;
/*
 * A linked list struct
 */
struct ListT {
     ListT* next;
    char* key;
    char* value;
};


struct HashTableT{
    int size;
    ListT** lists;
};



/*
 * Detroy a list structure. This functions free all the data allocated for that list
 */
void listDestroy(ListT* list){
    if(list->next!=NULL){
        listDestroy(list->next);
    }
    free(list);
}


/*
 * listCreate() creates a new instance of the List
 */
ListT* listCreate(char* key,char* value){
    ListT* list = (ListT*) malloc(sizeof(ListT));
    if(list==NULL){
        printf("Error: Cannot allocate memory for the List.");
        exit(-1);
    }else{
        list->next = NULL;
        list->key = key;
        list->value = value;
        return list;
    }
}
/*
 * addToList() adds data to the linked list
 */
void addToList( ListT ** list, char* key, char* value){
    if(*list == NULL){
        *list= listCreate(key,value);	
    }else{
        ListT** temp = &((*list)->next);
	
        while(*temp!= NULL){
            temp = &((*temp)->next);     
        }
        *temp = listCreate(key,value);
    }
} 
 
/*
 * listFind() will search the key in the list and return the value corresponding to that key
 */
char* listFind(ListT* list, char* key){
    if(list == NULL){
        return NULL;
    }else{
        if (strcmp(list->key,key)!=0){
            return listFind(list->next,key);
        }else{
            return list->value;
        }
    }
}



/*
 * HTCreate() function creates and returns a HashTable as a array of ListT* pointer.
 * The functions takes in the size of the hash table
 */
HashTableT* HTCreate(int size){

    HashTableT * ht = (HashTableT*) malloc(sizeof(HashTableT));
    if(ht==NULL){
         printf("Error: cannot allocate memory for the hash table.");
	 exit(-1);
    }else{
        ht-> size = size;
        ht->lists = (ListT**) malloc(size*sizeof(ListT*));
        memset(ht->lists,0,size*sizeof(ListT*));
	return ht;
    }
}

/*
 * hash() takes in a key as a string and the size of the hashtable, return the index number
 * where the the key will or can be located
 */
int hash(char*key,int size){
    int i = 0, index=0 ; 
    while(key[i] != '\0'){
        index+= (int) key[i++];
    }
    return index % size;

}

/*
 * addToHashTable adds a new element into the hash table
 */
void addToHashTable(HashTableT* ht,char* key, char* value){
    int index = hash(key,ht->size);
    addToList( &(ht->lists[index])  ,key,value);
}

/*
 * find searches for a value corresponding to the key string and returns it
 */
char* find(HashTableT* ht, char* key){	 
    int index = hash(key, ht->size);
    return listFind( ht->lists[index],key);
}

/*
 * HTDestroy will destroy the hash table and free all allocated memory associated with it
 */
void HTDestroy(HashTableT* ht){
    int i;
    for(i = 0 ; i < ht->size; i++){
        if(ht->lists[i]!=NULL)
            listDestroy(ht->lists[i]);
    }
    free(ht->lists);
    free(ht);
}

