/*
 * tokenizer.c
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "tokenizer.h"

typedef enum { false = 0, true } bool;  

struct TokenizerT_ {
    // The char pointer that points to the pre-allocated token string
    char* tokens;
    // Indicate current considered position of the Tokenizer
    char* curPos; 
};

TokenizerT *TKCreate( char * ts ) {
    // Allocate memory for the tokenizer
    TokenizerT *tokenizer = (TokenizerT*) calloc(1,sizeof(TokenizerT));
    if(tokenizer == NULL){
        return NULL;
   }else{
        int len = strlen(ts);
        // Copy the arguments
        tokenizer->tokens = (char*) malloc(len+1);
        strcpy(tokenizer->tokens, ts);
        //Set the initial position
        tokenizer->curPos = tokenizer->tokens;
        return tokenizer;
    }
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 */
void TKDestroy( TokenizerT * tk ) {
    // Free the copied string from the argument
    free(tk->tokens);
    // Free the memory of the tokenizer
    free(tk);
}

/*
 * addCurCharToToken shifts the token pointer by 1 char and add 1 to the length of the string
 *
 */
void addCurCharToToken(char** curPos, int * length){
    // Advance the pointer to the next position
    (*curPos)++;
    // Increase the length of the string
    (*length)++;
}
void formNextToken(char** curPos, int* length){
    addCurCharToToken(curPos,length);
    while( (**curPos!='\0')&& isalnum(**curPos)){
        addCurCharToToken(curPos,length);
    }
}
/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 */
char *TKGetNextToken( TokenizerT * tk ) {
    // Create the pointer to the first character 
    char* begin;
    // The length of the string
    int length = 0 ;
    //Flush all the white space character
    while(*tk->curPos!='\0' && !isalnum(*(tk->curPos))){
        tk->curPos++;
    }
    begin = tk->curPos;
    formNextToken(&(tk->curPos), &length); 
    char* token =(char* )  malloc(length+1);    
    strncpy(token,begin,length);
    token[length] = '\0';
    if(token[0]!='\0')
        return token;
    else{
        free (token);
        return NULL;
    }
}

