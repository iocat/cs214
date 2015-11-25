/*
 * tokenizer.c
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "tokenizer.h"
#define HASH_MAX 15
#define OPE_NO 42
#define KEYWORD_NO 32

typedef enum { false = 0, true } bool;  

struct TokenizerT_ {

    // The char pointer that points to the pre-allocated token string
    char* tokens;

    // Indicate current considered position of the Tokenizer
    char* curPos; 

    // Current Data Type of the token
    TokenT type;

};

TokenT TKGetType(TokenizerT* tk){
    return tk->type;
}
/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 */

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

/*
 * formFloatWithExpToken forms the token that is a floating number with the exponential part
 * The function returns the token type
 */
TokenT formFloatWithExpToken(char ** curPos , int * length){
    addCurCharToToken(curPos,length);
    if(**curPos == '+' || **curPos == '-'){
        addCurCharToToken(curPos,length);
        if(isdigit(**curPos)){
            addCurCharToToken(curPos,length);
            while(isdigit(**curPos)){
                addCurCharToToken(curPos,length);
            }
            return FLOAT; 
        }else{
            //mal
            return MAL;
        }
    }else if (isdigit(**curPos)){
        while(isdigit(**curPos)){
            addCurCharToToken(curPos, length);
        }
        return FLOAT;  
    }else{
        //mal
        return MAL;
    }
}

/*
 * formFloatWithExpToken forms the token that is a floating number which may or may not have 
 * the exponential part
 * The function returns the token type
 */

TokenT formFloatToken(char** curPos ,int* length){ 
    addCurCharToToken(curPos,length);
    while(isdigit(**curPos)){
        addCurCharToToken(curPos,length);
    }
    if(**curPos =='e'|| **curPos == 'E'){
        return formFloatWithExpToken(curPos,length);
    }else{
        return FLOAT;
    }
}

/*
 * formDecimalToken forms the token that is an decimal which could eventually become a floating
 * point number
 *
 * The function returns the token type
 */
TokenT formDecimalToken(char** curPos, int *length){
    while(isdigit(**curPos)){
        addCurCharToToken(curPos,length);
    }
    if(**curPos=='.'){
        return formFloatToken(curPos,length);
    }else if(**curPos == 'e' || **curPos== 'E'){
        return formFloatWithExpToken(curPos,length);
    }else{
        return DEC;
    } 
}

/*
 * formWordToken forms a word token
 *
 * The function returns the token type
 */
TokenT formWordToken(char** curPos, int* length){
    // Add the first alphabetic character to the token
    addCurCharToToken(curPos,length);
    //Keep adding alphanumeric char
    while(isalnum(**curPos)){
        addCurCharToToken(curPos,length);
    }
    return WORD;
}

/*
 * formHexToken forms a hexadecimal token if the current character is 'x' or 'X'
 *
 * The function returns the token type
 */
TokenT formHexToken(char** curPos, int* length){
    // Add the 'x' character to the token
    addCurCharToToken(curPos,length);

    if(isxdigit(**curPos)){
        addCurCharToToken(curPos,length);
        while(isxdigit(**curPos)){
            addCurCharToToken(curPos,length);
        }
        return HEX;
    }else{
        //mal 
        return MAL;
    }
}

/*
 * isodigit(int c) recognizes octal characters
 * if c is a octal digit, this function returns 1, else it returns 0
 */
int isodigit(int c){
    return '0'<= c && c<= '7';
}

/*
 * formOctalToken() forms a number token which could be an octal number 
 * It is called when the first character of the token is '0'
 * The function returns the token type
 */
TokenT formOctalToken(char** curPos, int* length){
    // If the leading character is 0.
    addCurCharToToken(curPos,length);
    if(**curPos =='x' || **curPos == 'X'){
        //To Hexadecimal branch
        return formHexToken(curPos,length);
    }else if(isodigit(**curPos)){
        // Octal branch 
        addCurCharToToken(curPos,length);
        while(isodigit( **curPos )){
            addCurCharToToken(curPos,length);
        }
        
        if('8'<= **curPos  && **curPos <='9'){
            // Move from octal branch to decimal branch
            return formDecimalToken(curPos, length);

        }else if (**curPos == '.'){
            // Move from octal branch to floating point branch
            return  formFloatToken(curPos, length);

        }else{
            return  OCTAL; 

        }
    }else if(**curPos=='.'){
        return formFloatToken(curPos,length);

    }else{
        //if it was just 0
        return  DEC;

    }
}
TokenT formNextControlToken(char**curPos,int* length, char** begin){
    addCurCharToToken(curPos,length);
    return CTR; 
}

TokenT formNextToken(char** curPos, int* length, char** begin);
TokenT formPuncToken(char** curPos, int* length,char** begin){
    switch(**curPos){
    case '.': 
	    addCurCharToToken(curPos, length);
            if(isdigit(**curPos)){
                //If the next char is a digit then switch to float branch
                return formFloatToken( curPos ,length);
            }
	    break;
    case '-':
  	    // Token "->"
	    if(*(*curPos+1)=='>'){
	    	addCurCharToToken(curPos,length);
		    addCurCharToToken(curPos,length);
		    break;
	    }
	case '+':
	case '&':
	case '|':
	    // Repeated chracter token : ++ && ||
	    if(*(*curPos+1)==**curPos){
	        addCurCharToToken(curPos,length);
	        addCurCharToToken(curPos,length);	
		    break;
	    }
	case '^':
	case '%':
	case '!':
	case '/':
        if(*(*curPos+1) == '/'){
            (*curPos)+=2;
            // Found '//', discard all characters until we found a new line character
            while(true){
                if(**curPos=='\0'){
                    break;
                }else if (**curPos == '\n'){
                    (*curPos)++;
                    break;
                }else{
                    (*curPos)++;
                }
            }
            return formNextToken(curPos,length,begin);
        }else if(*(*curPos+1) == '*'){
            (*curPos)+=2;
            // Found '/*', discards all characters until we found '*/'
            while(true){
                if (**curPos == '*'){
                    (*curPos)++;
                    if(**curPos == '/'){
                        (*curPos)++;
                        break;
                    }else{
                        continue;
                    }
                }else if(**curPos == '\0'){
                    break;
                }else{
                    (*curPos)++;
                }
            }
            return formNextToken(curPos,length,begin);   
        }
	case '*':
	case '=':
	    // All 2 character token ended with character '=' 
        addCurCharToToken(curPos,length);
       	if(**curPos == '='){
		    addCurCharToToken(curPos,length);
	    }
        break;
	case '>':
	case '<':
	    // Special cases for 3-character-token >>= <<= 
	    // Following tokens are checked: > < >= <= >> << >>= <<=
	    addCurCharToToken(curPos,length);
	    if(**curPos=='='){
		    addCurCharToToken(curPos,length);
        }else if(**curPos==*(*curPos-1)){
            addCurCharToToken(curPos,length);
		    if(**curPos=='='){
		        addCurCharToToken(curPos,length);
		    }
	    }
	    break;
    default:
	    // Default case: the other one character tokens
	    addCurCharToToken(curPos,length); 
    }
    return OPE;
}
TokenT formNextToken(char** curPos, int* length, char** begin){
    // flush all the blank characters
    while(isspace( **curPos)){
        (*curPos)++;     
    }
    
    *length = 0;
    *begin = *curPos;
    if(**curPos == '0'){
       // If the leading character is a 0 
       return  formOctalToken(curPos,length);

    }else if(isdigit(**curPos)){
        // If the leading character is a digit ( excluding 0 - already checked)
        return formDecimalToken(curPos,length);

    }else if(isalpha(**curPos)){
        // If the leading character is a alphabetic character
        return  formWordToken(curPos,length);

    }else if(ispunct(**curPos)){
        // If the leading character is a punctuation character
        return  formPuncToken(curPos,length, begin);
    }else if(iscntrl(**curPos) && **curPos!='\0'){
        // If the leading character is an escape sequence
        // print an error
        return formNextControlToken(curPos,length,begin);	
    }
    return 0;
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
    int length ;
    
    // Form the next token and return the type
    tk->type = formNextToken(&(tk->curPos), &length, &begin); 
    
    // Allocate the memory for the token based on its length
    char* token =(char* )  malloc(length+1);    

    // Copy the string from the token stream to the token 
    strncpy(token,begin,length);

    // Terminate the token string
    token[length] = '\0';

    // Check to see whether the token is a non-character sequence or not
    if(token[0]!='\0')
        return token;
    else{
        // if it is then free the allocated memory and return NULL instead
        free (token);
        return NULL;
    }
}

