#include <errno.h>
#include <stdio.h>
#include "hashmap.h"
#include <stdlib.h>
typedef struct map_t{
    void* key;
    void* value;
} map_t;

typedef struct node_t{
    map_t* map; 
    struct node_t* next;
}  node_t;

map_t* map_create (void* key, void* value){
    map_t* map = (map_t* ) malloc( sizeof(struct map_t));
    if(map == NULL){
        fprintf(stderr,"Cannot allocate memory for the map.%s\n",strerr(errno));
        
    }else{
        map->key = key;
        map->value = value;
        return map;
    }
}
void map_destroy(map_t* map){
    free(map);
}
node_t* node_create( map_t* map ){
    node_t* node = (node_t*) malloc( sizeof(struct node_t) );   
    if(node == NULL){
        fprintf(stderr,"Cannot allocate memory for the node.%s\n",strerr(errno));
    
    }else{
        node->map = map;
        node->next = NULL;
        return node;
    }
}
void node_destroy(node_t* node){
    map_destroy(node->map);
    free(node);
}
struct hashmap_t {
    int size;
    node_t** lists; 
    int(*hash)(void*);   
    int(*compare)(void*,void*);
};

hashmap_t* hm_create(int size, int(*hash_func)(void*),int(*compare_func)(void*,void*)){
    if(size<=0){
        fprintf(stderr,"Hash table size must be greater than 0.");
        return NULL;
    }else{    
        hashmap_t* hm = (hashmap_t*) malloc(sizeof( struct hashmap_t) );
        if(hm == NULL){
            fprintf(stderr,"Cannot allocate memory for the hashmap.%s\n",strerr(errno));
            return NULL;
        }else{
            hm->size = size;
            hm->lists = (node_t**) malloc(sizeof(node_t*)*size);
            hm->compare = compare_func;
            hm->hash = hash_func;
            return hm;
        }
    }
}
int hm_store(hashmap_t* hm, void* key, void* value){
    int index = hm->hash(key)%hm->size;
    if( hm->lists[index] == NULL){
        hm->lists[index] = node_create(map_create(key,value));
        return 1;
    }else{
        /* Go to the end of the list */
        node_t* node_ptr = hm->lists[index];
        while(node_ptr->next!=NULL){
            if(hm->compare(node_ptr->map->key,key)){
            // Key conflicts
                return 0;
            }else{
                node_ptr = node_ptr->next;
            }
        }
        node_ptr->next= (node_create(map_create(key,value)));
        return 1;
    }
}

void* hm_get(hashmap_t* hm, void* key){
    int index = hm->hash(key)%hm->size;
    node_t* node_ptr = hm->lists[index];
    while(node_ptr!=NULL && hm->compare(node_ptr->map->key, key)!=0){
        node_ptr = node_ptr->next;      
    }
    if(node_ptr == NULL){
        return NULL;
    }else{
        node_ptr->map->value;
    }
}
void* hm_remove(hashmap_t* hm, void* key ){
    int index = hm->hash(key)%hm->size;
    node_t* node_ptr = hm->lists[index];
    if(node_ptr == NULL){
        return NULL;
    }else{
        node_t* prev = NULL;
        while(node_ptr!=NULL && hm->compare(node_ptr->map->key, key)!=0){
            prev = node_ptr;
            node_ptr = node_ptr->next;
        }
        if( node_ptr == NULL){
            return NULL;
        }else if(prev==NULL){
            hm->lists[index] = node_ptr->next;
        }else{
            prev= node_ptr->next;
        }
        void* value = node_ptr->map->value;
        node_destroy(node_ptr);
    }
}
void recursively_destroy_node(node_t* node){
    if(node == NULL){
        return;
    }else{
        recursively_destroy_node(node->next);
        node_destroy(node);
    }
}
void hm_destroy(hashmap_t * hm){
    int i = 0;
    for(i = 0 ; i < hm->size;i++){
        recursively_destroy_node(hm->lists[i]);
    }      
}
