#include "client-command.h"
#include "tokenizer.h"
#include "account.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PROMPT ">>>"
#define INVALID_CMD "Invalid commnad. Use help to get more information."
// Wait time between 2 commands
#define WAIT_TIME 2
void* command_subroutine(void* arg){
    command_arg_t* command_arg = (command_arg_t*) arg;
    int client_socket_fd = command_arg->client_socket_fd;
    char user_input[200];
    char* token;
    request_t request;
    int valid_command;
    printf(PROMPT);
    while(fgets(user_input,sizeof(user_input),stdin)!=NULL){
        valid_command = 0;
        TokenizerT* tk =  TKCreate(user_input);
        token = TKGetNextToken(tk);
        
        if( strcmp(token,"open") == 0 ){
            free(token);
            token = TKGetNextToken(tk);
            if(strlen(token)<= NAME_MAX_LENGTH-1){
                request.code = htonl(OPEN);
                strcpy(request.message.name,token);
                valid_command = 1;
            }else{
                printf("The name length is invalid.\n");
            }
            free(token);
        }else if(strcmp(token,"start")==0){
            free(token);
            token = TKGetNextToken(tk);
            if(token == NULL){
                printf("%s\n",INVALID_CMD);
            }else{
                if(strlen(token)<=NAME_MAX_LENGTH-1){
                    request.code = htonl(START);
                    strcpy(request.message.name,token);
                    valid_command = 1;
                }else{
                    printf("The name length is invalid.\n");
                }
                free(token);
            }
        }else if(strcmp(token,"credit")==0){
            free(token);
            token = TKGetNextToken(tk);
            if(token== NULL || TKGetType(tk)!=FLOAT){
                printf("%s\n",INVALID_CMD);
            }else {
                request.code = htonl(CREDIT);
                request.message.amount = htonl(atof(token));
                valid_command = 1;
                free(token);
            }
        }else if(strcmp(token,"debit")==0){
            free(token);
            token = TKGetNextToken(tk);
            if(token== NULL || TKGetType(tk)!=FLOAT){
                printf("%s\n",INVALID_CMD);
            }else {
                request.code = htonl(DEBIT);
                request.message.amount = htonl(atof(token));
                valid_command = 1;
                free(token);
            } 
        }else if(strcmp(token,"balance")==0){

        }else if(strcmp(token,"finish")==0){

        }else if(strcmp(token,"exit")==0){

        }else if(strcmp(token,"help")==0){
            free(token);
            printf("open ACCOUNT_NAME\n"\
                   "start ACCOUNT_NAME\n"\
                   "credit AMOUNT\n"\
                   "debit AMOUNT\n"\
                   "balance\n"\
                   "finish\n"\
                   "exit\n");
        }else{
            free(token);
            printf("The command is invalid. Use 'help' to see a "\
                    "list of command options.");
        }
        
        if(valid_command){
            write(client_socket_fd,(void*)&request,sizeof( request_t));
            sleep(WAIT_TIME);
        }
        TKDestroy(tk);
        printf(PROMPT);
    } 

    pthread_exit(NULL);
}

