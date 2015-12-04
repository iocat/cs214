#include <sys/mman.h>
#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TIME_OUT 5000
#include "account.h"
#include "server-client.h"
#include <unistd.h>
int search_account(account_t* accounts, int account_no, char* name){
    int index = 0 ; 
    if(account_no == 0){
        return -1;
    }else{
        for(index = 0 ; index < account_no; index++){
            if(strcmp(accounts[index].name,name)==0){
                break;
            }
        }
        return index;
    }
}
void client_open(response_t* response,client_t* c,char* name){
   int index;
   pthread_mutex_lock( c->new_account_lock_mutex_ptr);
   if(*c->accounts_no_ptr == MAX_ACCOUNT){
        form_response(response,CANNOT_OPEN,"Database is full.");
   }else{
        index = search_account(c->accounts,*c->accounts_no_ptr,name);
        if(index == -1){
            account_open(&c->accounts[0],name);
        }else if(index <= *c->accounts_no_ptr -1){
            form_response(response,CANNOT_OPEN,"Account's already created.");
            pthread_mutex_unlock(c->new_account_lock_mutex_ptr);
            return;
        }else{
            account_open(&c->accounts[index],name);
        }
        (*c->accounts_no_ptr)++;

   }
   pthread_mutex_unlock(c->new_account_lock_mutex_ptr);
   char message [200];
   sprintf(message,"Account %s opened",name);
   form_response(response,SUCCESS,message);
}

void client_start(response_t* response,client_t* c,account_t** cli_acc 
        ,char* name){
    int index;
    pthread_mutex_lock(c->new_account_lock_mutex_ptr);
    index = search_account(c->accounts,*c->accounts_no_ptr,name);
    if(index == -1 || index == *c->accounts_no_ptr){
        form_response(response,CANNOT_START,"Account is not found.");
    }else{
        account_t* account = & c->accounts[index];
        int in_session = 0;
        pthread_mutex_lock(&account->account_mutex);
        if(account->in_session == IN_SESSION){
            in_session = 1;
        }else{
            in_session = 0;
            account->in_session = IN_SESSION;
        }
        pthread_mutex_unlock(&account->account_mutex);

        if(in_session){
            form_response(response,CANNOT_START,"Requested account has been"\
                    " started on another session");
        }else{
            *cli_acc = account;
            char message [200];
            sprintf(message,"Account %s is in session.",name);
            form_response(response,SUCCESS,message);
        }
    }
    pthread_mutex_unlock(c->new_account_lock_mutex_ptr);
}

void client_debit(response_t* response,account_t* account, char* amount){
    char message[100];
    int result = account_debit(account,atof(amount));
    if(result == BALANCE_REACH_ZERO){
        sprintf(message,"Balance reached 0. Your balance is: $%.2f",
                account->balance);
        form_response(response,result, message);
    }else if(result == SUCCESS){
        sprintf(message,"Successfully debit $%s.",amount);
        form_response(response,result,message);
    }
}
void client_credit(response_t* response, account_t* account, char* amount){
    char message[100];
    account_credit(account,atof(amount));
    sprintf(message,"Successfully credit $%s.",amount);
    form_response(response,SUCCESS,message);
}
void client(client_t* client_dat,int client_sock_fd){
    // The account of the active client
    account_t* client_account = NULL;

    // Set up parameters for select
    struct timeval timeout  ={ TIME_OUT, 0 }; // Timeout after 5000 seconds
    fd_set read_fd_set;
    FD_ZERO(&read_fd_set); 
    FD_SET(client_sock_fd, &read_fd_set);

    int active_sockets;

    request_t request;
    response_t response;
    
    while((active_sockets = select(client_sock_fd+1,&read_fd_set,NULL,NULL,
                    &timeout))>0){
        int byte_read = listen_request(&request,client_sock_fd);
        if(byte_read == 0){
            // Client closed connection unexpectedly. (not manually)
            if(client_account != NULL){
                account_set_in_session(client_account, NOT_IN_SESSION);
                client_account = NULL;
            }
            break;
        }
    
        if(client_account == NULL || !account_is_in_session(client_account)){
            // Current client did not open the 
            switch(request.code){
                case OPEN:
                   client_open(&response,client_dat,request.message.name);
                   break; 
                case START:
                    client_start(&response,client_dat,&client_account
                           ,request.message.name);            
                    break;
                case EXIT:
                    form_response(&response,ACCOUNT_EXIT,"Exit acknowledged.");
                    break;
                default:
                    form_response(&response,CLIENT_NOT_IN_SESSION,"Please "\
                            "start an account before doing the transaction.");
                    break;
            }
        }else{
            switch(request.code){
                case OPEN:
                    form_response(&response,CANNOT_OPEN,"Please finish this"\
                            " account session first.");
                    break;
                case START:
                    form_response(&response,CANNOT_START,"Please finish this"\
                            " account session first.");
                    break;
                case DEBIT:
                    client_debit(&response,client_account,request.message.amount);
                    break;
                case CREDIT:
                    client_credit(&response,client_account,request.message.amount);
                    break;
                case BALANCE:
                    form_response(&response,SUCCESS,"");
                    sprintf(response.message,"Account: %s.\nYour balance is %.2f.",
                            client_account->name,client_account->balance);
                case FINISH:
                    account_set_in_session(client_account,NOT_IN_SESSION);
                    form_response(&response,SUCCESS,"Account session closed.");
                    client_account = NULL;
                    break;
                case EXIT:
                    account_set_in_session(client_account,NOT_IN_SESSION);
                    form_response(&response,SUCCESS,"Exit acknowledged and "\
                            "Account session closed.");
                    client_account = NULL;
                    break;
                default: 
                    form_response(&response,SUCCESS,"Not a valid request.");
                    break;
            }

        }
        // Send the response to the client without failing!
        while(write(client_sock_fd,&response,sizeof(response_t))<0){
            continue;
        }
        if(request.code == EXIT){
            break;
        }
    }
    if(active_sockets == 0 ){
        if (client_account!=NULL){
            client_account = NULL;
        }
        form_response(&response, CONNECTION_TIME_OUT,"Connection time out.");
        write(client_sock_fd, &response,sizeof(response_t));
    }
    exit(EXIT_SUCCESS);
}
