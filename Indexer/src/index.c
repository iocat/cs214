#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "dirutil.h"
#include "hashmap.h"
#include "sorted-list.h"
#include "tokenizer.h"
#include <ctype.h>
#ifdef MALLOC
    #include "EnhancedMalloc/heap.h"
#endif
#define HASH_SIZE 100
#define PROGRAM_NAME "index"
void terminate(char* error_mes){
    fprintf(stderr,"%s: %s\n",PROGRAM_NAME, error_mes);
    exit(EXIT_FAILURE);
}
typedef struct record_t {
    char* file;
    int count;
} record_t;

record_t* create_record(char* file,int count){
    record_t* new_record = (record_t*) malloc(sizeof(record_t));
    if(new_record== NULL){
        terminate("Cannnot allocate memory for the new record.");
        return NULL;
    }else{
        new_record->file = (char* ) calloc( strlen(file)+1,sizeof(char)); 
        if(new_record->file==NULL){
            terminate("Cannot allocate memory for the file name.");
        }
        strcpy(new_record->file,file);
        new_record->count = count;
        return new_record;
    }
}
int compare_record(void* record1, void* record2 ){
    record_t * recordPtr1 = (record_t*) record1;
    record_t * recordPtr2 = (record_t*) record2;
    // Sorted in descending order 
    int comp;
    if(recordPtr1->count == recordPtr2->count){
        comp = strcmp(recordPtr2->file,recordPtr1->file);
    }else{
        comp = recordPtr1->count - recordPtr2->count;
    }
    if(comp>0 ){
        return 1;
    }else if(comp < 0){
        return -1;
    }else return 0;
}
void destruct_record(void* record){
    free(((record_t*)record)->file);
    free(record);
}

typedef struct index_t {
    char* token;
    SortedListPtr records;
} index_t;

index_t* create_index(char* token){
    index_t* new_index = (index_t*) malloc(sizeof( index_t));
    if(new_index == NULL){
        terminate("Cannot allocate memory for the index.");
        return NULL;
    }else{
        new_index->token = (char*) calloc(strlen(token)+1,sizeof(char)); 
        if(new_index->token==NULL){
            terminate("Cannot allocate memory for the index's token.");
        }
        strcpy(new_index->token, token);
        new_index->records = SLCreate(compare_record,destruct_record);
        if(new_index->records==NULL){
            terminate("Cannot allocate memory for the index's record list.");
        }
        return new_index;
    }
}
int compare_index(void* index1, void* index2){
    index_t * indexPtr1 = (index_t*) index1;
    index_t * indexPtr2 = (index_t*) index2;   
    //Sorted in ascending order-> twist
    int comp =  strcmp( indexPtr2->token , indexPtr1->token);
    if(comp >0){
        return 1;
    }else if(comp<0){
        return -1;
    }else   return 0;
}
void destruct_index(void* index){
    index_t* indexPtr = (struct index_t*) index;
    free(indexPtr->token);
    SLDestroy(indexPtr->records);
    free(indexPtr);
}

void usage(){
    printf("usage: index <inverted-index file name> <directory or file name>\n");
}

index_t* find_index(SortedListPtr indeces,char* token){
    SortedListIteratorPtr sli = SLCreateIterator(indeces);
    index_t* temp = (index_t*) SLGetItem(sli);
    while(temp!=NULL){
        if(strcmp( temp->token,token)==0){
            SLDestroyIterator(sli);
            return temp;
        }else{
            temp = SLNextItem(sli);
        }
    }
    SLDestroyIterator(sli);
    return NULL;
}

/* Fix the file path*/
char* fix_dir_path(char* dir_path){
    int length = strlen(dir_path);
    int new_length = length;
    if(dir_path[length-1] != '/'){
        new_length+=1;
    }
    char* new_dir_path = (char*) malloc(new_length+1);
    if(new_dir_path==NULL){
        terminate("Cannot allocate memory for new path");
    }
    strcpy(new_dir_path,dir_path);
    if(new_length>length){
        strcat(new_dir_path,"/");
    }
    #ifdef DEBUG
    printf("Fix dir path %s:%lu to %s:%lu\n",dir_path,strlen(dir_path),new_dir_path,strlen(new_dir_path));
    #endif
    return new_dir_path;
}
int compare_string(void* str1, void* str2){
    char* s1 = (char*) str1;
    char* s2 = (char*) str2;
    return strcmp(s1,s2);
}
int hash_string(void* str){
    char* string = (char*)str;
    int length = strlen(string);
    int i;
    int hash=0 ; 
    for(i = 0 ; i<length; i++){
        hash += string[i];
    }
    return hash;
}
hashmap_t* tokenize_file(char* file_path){
    char* file_buffer = regular_file_to_string(file_path);
    TokenizerT* tk = TKCreate(file_buffer);
    free(file_buffer);
    #ifdef DEBUG
    printf("Tokenize %s:\n",file_path);
    #endif
    // Create a hashmap
    hashmap_t* hm = hm_create(HASH_SIZE,hash_string,compare_string);
    // For each token read from file store the count to hashmap
    char* token;
    while((token=TKGetNextToken(tk))!=NULL){
        if(TKGetType(tk) == WORD){
            // Transform token to lower case
            int _count=0;
            for(_count = 0 ; _count < strlen(token); _count++){
                token[_count] = tolower(token[_count]);
            }
            map_t* token_count =hm_get(hm,token);
            // If the token is not found
            if(token_count == NULL){
                // Store a map key - value
                int* i = (int*)malloc(sizeof(int)); 
                if(i == NULL){
                    terminate("Cannot allocate memory.");
                }else{
                    *i = 1;
                    hm_store(hm,token,i);  
                }
                 #if DEBUG
                printf("Create new map: %s:%d\n",token,*i);
                printf("\tCurrent dictionary contains:\n");
                hm_iterator_t* _hmi = hm_create_iterator(hm); 
                map_t* _map = hmi_get(_hmi);
                while(_map!=NULL){
                    printf("\t\t%s:%d\n",(char*) _map->key, *(int*) _map->value);
                    _map = hmi_get_next(_hmi);
                }
                printf("\n");
                hmi_destroy(_hmi);
                #endif
            }else{
                // Otherwise, free the found token in file and increase the token count by 1
                free(token);
                int* value = token_count->value;
                (*value)+=1;
                #ifdef DEBUG
                printf("Increase token count %s - %d\n",(char*) token_count-> key,*value);
                #endif
            }
        }else{
            free(token);
        }
    }
    TKDestroy(tk);
    return hm;
}

void index_file(char* offset_path,char* file_path,SortedListPtr indeces ){
    // Create a file buffer from the file path
    int offset_path_len = strlen(offset_path);
    int file_path_len = strlen(file_path);
    char* tempPtr = &file_path[offset_path_len];
    // Cut the file_path and form the relative path
    char* relative_path = malloc(file_path_len-offset_path_len+1);
    if(relative_path == NULL){
        terminate("Cannot allocate memory.");
    }
    strcpy(relative_path,tempPtr);
    #ifdef DEBUG
    printf("Indexing %s\n",relative_path);
    #endif
    //First make a dictionary of lowercase-token:count for this file
    hashmap_t* hm = tokenize_file(file_path);
    // For each token-counts set transform it into token_(file:count) 
    hm_iterator_t* hmi = hm_create_iterator(hm);
    map_t* temp;
    while((temp=hmi_get_next(hmi)) != NULL){
        index_t* found_index= find_index( indeces, (char*) temp->key); 
        record_t* new_record = (record_t*) create_record(relative_path,*(int*)temp->value);
        if(found_index == NULL){
            //Index unforund
            index_t* new_index = create_index((char*)temp->key);
            SLInsert(new_index->records,(void*) new_record);
            SLInsert(indeces, (void*) new_index);
            #ifdef DEBUG
            printf("| Insert new record: %s - %d to newly created index %s\n",relative_path,
                    *(int*) temp->value, /*new_index->token*/(char*) temp->key );
            #endif
        }else{ 
            SLInsert(found_index->records,new_record); 
            #ifdef DEBUG
            printf("| Insert new record %s - %d to index %s\n",relative_path,*(int*) temp->value,
                    found_index->token);
            #endif
        }
        free(temp->key);
        free(temp->value);
    } 
    hmi_destroy(hmi);
    hm_destroy(hm);
    free(relative_path);
}

void write_to_file(char* file_path, SortedListPtr indeces){
    #ifdef DEBUG
    printf("Write to file %s:\n",file_path);
    #endif
    printf("{\"list\" : [\n");
    SortedListIteratorPtr index_iter = SLCreateIterator(indeces);
    index_t* index_temp = (index_t*) SLGetItem(index_iter);
    while(index_temp!=NULL){
        SortedListIteratorPtr record_iter = SLCreateIterator(index_temp->records);
        record_t* record_temp = (record_t*) SLGetItem(record_iter);
        printf("\t{\"%s\" : [\n",index_temp->token);
        while(record_temp!=NULL){
            printf("\t\t{\"%s\" : %d}",record_temp->file,record_temp->count);
            record_temp =(record_t*) SLNextItem(record_iter);
            if(record_temp== NULL){
                printf("\n");
            }else{
                printf(",\n");
            }
        }
        printf("\t]}");
        SLDestroyIterator(record_iter);
        index_temp = (index_t*) SLNextItem(index_iter);
        if(index_temp == NULL){
            printf("\n");
        }else{
            printf(",\n");
        }
    }
    printf("]}\n");
    SLDestroyIterator(index_iter);
}
void traverse_dir(char* dir_path, SortedListPtr indeces){
    static int initialized = 0;
    static char* offset_path;
    if(!initialized){
        offset_path = dir_path;
        initialized = 1;
        #ifdef DEBUG
        printf("The offset path is :%s\n",offset_path);
        printf("Inspect dir: %s\n",offset_path);
        #endif
    }
    DIR* dir_stream = opendir(dir_path);
    struct dirent* dir_ent = readdir(dir_stream);
    char* new_path;
    while(dir_ent!=NULL){
        if(strcmp(dir_ent->d_name,".")!= 0 && strcmp(dir_ent->d_name,"..")!=0){
            /* Append the the current dir path */
            new_path = (char*)malloc(strlen(dir_path) + strlen(dir_ent->d_name) +1);
            if(new_path==NULL){
                terminate("Cannot allocate memory for the string.");
            }
            strcpy(new_path,dir_path);
            strcat(new_path,dir_ent->d_name);
            /* Check the current entry  */
            if(dir_ent->d_type==DT_DIR){
                char* fixed_path = fix_dir_path(new_path);   
                traverse_dir(fixed_path,indeces); 
                #ifdef DEBUG  
                printf("Go to dir:%s\n",fixed_path);
                #endif
                free(fixed_path);
            }else if(dir_ent->d_type==DT_REG){
                index_file(offset_path,new_path,indeces);   
            }
            free(new_path);
        }
        // Go to the next entry
        dir_ent = readdir(dir_stream);
    }
    closedir(dir_stream);
}


void check_inverted_index_file(char* inverted_index_file){
    /* Check index file to store the inverted indeces */
    if(exists(inverted_index_file)){
        if(is_dir(inverted_index_file)){
            terminate("The given inverted-index file already exists as a directory."); 
        }else{
            char c;
            printf("%s already existed. Do you wanna overwrite it? [Y|n]\n",inverted_index_file);
            scanf("%c",&c);
            if (c != 'Y' || c!='y' || c == 'N' || c == 'n'){
                exit( EXIT_FAILURE);
            }
        }
    }

}

enum I_Type {I_NOT_EXIST=-1, I_DIR, I_FILE };
enum I_Type check_file_path(char* file_path){
    /* Check the file/dir to be indexed */
    if(exists(file_path)){
        if(is_regular_file(file_path) ){
            /* The given path is a file */
            return I_FILE;
        } else if(is_dir(file_path)){
            /* The given path is a directory */
            return I_DIR;
        }else{
            terminate("Invalid file/directory to be indexed.");  
        }
    }else{
        terminate("File/Directory to be indexed does not exist.");
    }
    return I_NOT_EXIST;
}
void clean_up_before_exit(int exit_status, void* index_list){
    if(exit_status == EXIT_FAILURE){
        SLDestroy((SortedListPtr)index_list);
    }
}

int main(int argc , char* argv[]){
    if( argc == 1){
        usage();
    }else if(argc ==2 ){
        terminate("Missing file operand.");
    }else{

        SortedListPtr indeces = SLCreate(compare_index, destruct_index);
        on_exit(clean_up_before_exit,indeces);
    
        char* inverted_index_file = argv[1];
        char* file_path = argv[2];
        enum I_Type file_type;
        
        /* Check the pre-conditions */
        check_inverted_index_file(inverted_index_file);
        file_type = check_file_path(file_path);  
        /* Handle given file/dir */
        if(file_type == I_FILE){
             index_file("",file_path,indeces);    
        }else if(file_type == I_DIR){
            char* fixed_dir_path = fix_dir_path(file_path);
            traverse_dir(fixed_dir_path, indeces);
            free(fixed_dir_path);
        } 
        /* Write to file */
        write_to_file(inverted_index_file,indeces);
        /* Destroy the List */
        SLDestroy(indeces); 
    }
    return EXIT_SUCCESS;
}
