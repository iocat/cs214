/*
 * A hashmable uses seperate chaining technique to solve collisions. Implemented using the 
 * SortedList
 *
 */
typedef struct hashmap_t hashmap_t;

hashmap_t* hm_create(int size,int(*hash_func)(void*) ,
        int(*compare_func)(void*,void* ));
int hm_store( hashmap_t* hm,void * key, void* value);
void* hm_get( hashmap_t* hm, void* key  );
void* hm_remove(hashmap_t* hm, void* key);
void hm_destroy( hashmap_t* hm);
