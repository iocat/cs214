#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char * argv[]){
    if(argc == 1){
        printf("Need arguments");
    }else{
        TokenizerT* tokenizer = TKCreate(argv[1]);
        char* token;
        while((token=TKGetNextToken(tokenizer))!=NULL){
            printf("%s\n",token);
            free(token);
        }   
    }
    return EXIT_SUCCESS;

}
