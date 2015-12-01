#include "client-command.h"
#include "tokenizer.h"
#include "account.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#define PROMPT ">>> "
#define INVALID_CMD "Invalid command. Use help to get more information."
// Wait time between 2 commands
#define WAIT_TIME 2
#define TERMIOS_ECHO 1
#define TERMIOS_NON_ECHO 0
static struct termios old, new;

void initTermios(int echo) {
    /* grab old terminal i/o settings */
    tcgetattr(0, &old);                        
    /* make new settings same as old settings */
    new = old;            
    /* disable buffered i/o */
    new.c_lflag &= ~ICANON;                       
    /* set echo mode */
    new.c_lflag &= echo ? ECHO : ~ECHO;
    /* use these new terminal i/o settings now */
    tcsetattr(0, TCSANOW, &new);              
}

void resetTermios(void) {
      tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo) {
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

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
        if(token==NULL){
            valid_command = 0;
        }else if( strcmp(token,"open") == 0 ){
            free(token);
            token = TKGetNextToken(tk);
            if(token==NULL){
                printf(INVALID_CMD);
            }else{
                if(strlen(token)<= NAME_MAX_LENGTH-1){
                    request.code = htonl(OPEN);
                    strcpy(request.message.name,token);
                    valid_command = 1;
                }else{
                    printf("The name length is invalid.\n");
                }
                free(token);
            }
        }else if(strcmp(token,"start")==0){
            free(token);
            token = TKGetNextToken(tk);
            if(token == NULL){
                valid_command = 0 ;
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
            if(token== NULL){
                printf("%s\n",INVALID_CMD);
            }else if(TKGetType(tk)==FLOAT || TKGetType(tk)==DEC) {
                request.code = htonl(CREDIT);
                strcpy(request.message.amount,token);
                valid_command = 1;
                free(token);
            }
        }else if(strcmp(token,"debit")==0){
            free(token);
            token = TKGetNextToken(tk);
            if(token== NULL){
                printf("%s\n",INVALID_CMD);
            }else if(TKGetType(tk)==FLOAT || TKGetType(tk)==DEC){
                request.code = htonl(DEBIT);
                strcpy(request.message.amount,token);
                valid_command = 1;
                free(token);
            }else{
                printf("%s\n",INVALID_CMD);
                free(token);
            } 
        }else if(strcmp(token,"balance")==0){
            free(token);
            request.code = htonl(BALANCE);
            valid_command=1;
        }else if(strcmp(token,"finish")==0){
            free(token);
            request.code = htonl(FINISH);
            valid_command=1;
        }else if(strcmp(token,"exit")==0){
            free(token);
            request.code = htonl(EXIT);
            valid_command = 1;
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
                    "list of command options.\n");
        }        
        if(valid_command){
            if((token=TKGetNextToken(tk))!=NULL){
                printf("Invalid command.\n");
                free(token); 
            }else{
                write(client_socket_fd,(void*)&request,sizeof( request_t));
                initTermios(TERMIOS_NON_ECHO);
                sleep(WAIT_TIME);
                lseek(0,0,SEEK_END);
                resetTermios();
            }
        }
        TKDestroy(tk);
        if(request.code ==ntohl(EXIT)){
            break;
        }else{
            printf(PROMPT);
        }
    } 
    close(client_socket_fd);
    pthread_exit(NULL);
}

