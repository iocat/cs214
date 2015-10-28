#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "dirutil.h"
#include "SortedList/sorted-list.h"
#include "Tokenizer/tokenizer.h"
#include <ctype.h>
#ifdef MALLOC
    #include "EnhancedMalloc/heap.h"
#endif


#define PROGRAM_NAME "index"
typedef enum {false =0, true} bool;
struct Index {
    char* token;
    SortedListPtr records;
};
struct Record {
    char* file;
    int count;
};
int compare_index(void* index1, void* index2){
    struct Index * indexPtr1 = (struct Index*) index1;
    struct Index * indexPtr2 = (struct Index*) index2;   
    //Sorted in ascending order-> twist
    return strcmp(indexPtr2->token, indexPtr1->token);
}
void destruct_index(void* index){
    struct Index* indexPtr = (struct Index*) index;
    free(indexPtr->token);
    SLDestroy(indexPtr->records);
}
int compare_record(void* record1, void* record2 ){
    struct Record * recordPtr1 = (struct Record*) record1;
    struct Record * recordPtr2 = (struct Record*) record2;
    // Sorted in descending order 
   return strcmp(recordPtr1->file, recordPtr2->file);
}
void destruct_record(void* record){
    free(((struct Record*)record)->file);
}
void terminate(char* error_mes){
    fprintf(stderr,"%s: %s\n",PROGRAM_NAME, error_mes);
    exit(EXIT_FAILURE);
}
void usage(){
    printf("usage: index <inverted-index file name> <directory or file name>\n");
}

struct Index* findIndex(SortedListPtr indeces,char* token){
    struct Index temp = {token,NULL};
    SortedListIteratorPtr sli = SLCreateIterator(indeces);
    if(SLGetItem(sli)==NULL){
        return NULL;
    }
    do{ 
        if(compare_index(SLGetItem(sli),(void*) &temp)){
            return (struct Index*) SLGetItem(sli);
        }   
    }while(SLNextItem(sli)!=NULL);
    return NULL;
}

/* Fix the file path*/
char* fix_dir_path(char* dir_path){
    int length = strlen(dir_path);
    int new_length = length;
    char* new_dir_path;
    if(dir_path[length-1] != '/'){
        new_length+=1;
    }
    new_dir_path = (char*) malloc(new_length+1);
    if(new_dir_path==NULL){
        terminate("Cannot allocate memory for new path");
    }
    strcpy(new_dir_path,dir_path);
    if(new_length>length){
        strcat(new_dir_path,"/");
    }
    return new_dir_path;
}
void index_file(char* offset_path,char* file_path,SortedListPtr indexes ){
    // Create a file buffer from the file path
    char* file_buffer = regular_file_to_string(file_path);
   //First make a dictionary of lowercase-token:count for this file
    
    //Secondly, for each token create a new record : "file: count" and push them to 
    //the corresponding token's index
    
    free (file_buffer);
}

void traverse_dir(char* dir_path, SortedListPtr indeces){
    static bool initialized = false;
    static char* offset_path;
    if(initialized == false){
        offset_path = dir_path;
        initialized = true;
    }
    #ifdef DEBUG
    printf("Go to dir: %s\n",dir_path);
    #endif
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
            if(is_dir(new_path)){
                char* fixed_path = fix_dir_path(new_path);   
                traverse_dir(fixed_path,indeces); 
                #ifdef DEBUG  
                printf("%s\n",fixed_path);
                #endif
                free(fixed_path);
            }else if(is_regular_file(new_path)){
                //index_file(offset_path,new_path,indeces);   
                #ifdef DEBUG
                printf("%s\n",new_path);
                #endif
            }
            free(new_path);
        }
        // Go to the next entry
        dir_ent = readdir(dir_stream);
    }
}

void check_indexer_file(char* index_file){
    /* Check index file to store the inverted indeces */
    if(exists(index_file)){
        if(is_dir(index_file)){
            terminate("The given inverted-index file existed as a dir\n"); 
        }else{
            char c;
            printf("%s already existed. Do you wanna overwrite it? [Y|n]\n",index_file);
            scanf("%c",&c);
            if (c != 'Y' || c!='y' || c == 'N' || c == 'n'){
                exit( EXIT_FAILURE);
            }
        }
    }

}

enum I_Type { I_DIR, I_FILE };
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
            terminate("Invalid file/directory to be indexed\n");  
        }
    }else{
        terminate("File does not exist to be indexed");
    }
    return -1;
}

int main(int argc , char* argv[]){
    if( argc == 1){
        usage();
    }else if(argc ==2 ){
        terminate("Missing file operand.");
    }else{
        SortedListPtr indeces = SLCreate(compare_index, destruct_index);
        char* index_file = argv[1];
        char* file_path = argv[2];
        char* fixed_dir_path ;
        enum I_Type file_type;

        check_indexer_file(index_file);
        file_type= check_file_path(file_path);  
        if(file_type == I_FILE){
                    
        }else if(file_type == I_DIR){
            fixed_dir_path = fix_dir_path(file_path);
            traverse_dir(fixed_dir_path, indeces);
            free(fixed_dir_path);
        } 
        SLDestroy(indeces); 
    }
    return EXIT_SUCCESS;
}
