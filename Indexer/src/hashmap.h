/*
 * A hashmable uses seperate chaining technique to solve collisions. Implemented using the 
 * SortedList
 *
 */
typedef struct hashmap_t hashmap_t;
typedef struct map_t{
    void* key;
    void* value;
}map_t;
hashmap_t* hm_create(int size,int(*hash_func)(void*) ,
        int(*compare_func)(void*,void* ));
int hm_store( hashmap_t* hm,void * key, void* value);
map_t* hm_get( hashmap_t* hm, void* key  );
void* hm_remove(hashmap_t* hm, void* key);
void hm_destroy( hashmap_t* hm);

typedef struct hm_iterator_t hm_iterator_t;
hm_iterator_t* hm_create_iterator(hashmap_t* hm);
map_t* hmi_get_next(hm_iterator_t*);
map_t* hmi_get(hm_iterator_t*);
void hmi_destroy(hm_iterator_t*);
