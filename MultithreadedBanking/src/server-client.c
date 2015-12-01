#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "server-client.h"

void set_response(response_t* response ,int code, char* message){
    response->code = htonl(code);
    strcpy(response->message, message);
}
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
#define client_socket_fd client_thread_arg->client_socket_fd
#define accounts client_thread_arg->accounts
#define accounts_no_ptr client_thread_arg->accounts_no_ptr
#define accounts_max client_thread_arg->accounts_max
#define client_account client_thread_arg->client_account
#define new_account_lock_mutex_ptr client_thread_arg->new_account_lock_mutex_ptr
void* client_subroutine(void* arg){
    client_thread_info_t* client_thread_arg = (client_thread_info_t*) arg;

    request_t request;
    response_t response;

    struct timeval timeout  ={ 1200, 0 }; /* wait 20 minutes till timeout */
    fd_set read_fd_set;
    FD_ZERO(&read_fd_set); 
    FD_SET(client_socket_fd, &read_fd_set);
    int index =0 ;
    int active_sockets;
    int byte_read;
    while((active_sockets = select(client_socket_fd+1,&read_fd_set,NULL,NULL,
                    &timeout))>0){
        /* Read the request from client */
        byte_read = read(client_socket_fd, &request, sizeof(request_t));
        if(byte_read == 0){
            printf("Connection with client (socket id %d) is closed.\n",client_socket_fd);
            if(client_account != NULL){
                account_set_in_session(client_account, NOT_IN_SESSION);
                client_account = NULL;
            }
            break;
        }
        // Not in customer session 
        if(client_account == NULL || !account_is_in_session(client_account)){
            switch (ntohl(request.code)){
            case OPEN:
                pthread_mutex_lock(new_account_lock_mutex_ptr);
                if(*accounts_no_ptr == accounts_max){
                    set_response(&response,CANNOT_OPEN,"Database is full.");
                }else{ 
                    index = search_account(accounts,*accounts_no_ptr,
                         request.message.name);
                    if(index==-1){
                        (*accounts_no_ptr)++;
                        account_open(&accounts[0],request.message.name);
                    }else if(index <= *accounts_no_ptr - 1){
                        set_response(&response,CANNOT_OPEN,"Account's already created.");
                        pthread_mutex_unlock(new_account_lock_mutex_ptr);
                        break;
                    }else{
                        account_open(&(accounts[index]),request.message.name);
                        (*accounts_no_ptr)++;
                    }           
                    set_response(&response,SUCCESS,"");
                    sprintf(response.message,"Account %s opened.",
                            request.message.name);
                }
                pthread_mutex_unlock(new_account_lock_mutex_ptr);
                break;
            case START:
                pthread_mutex_lock(new_account_lock_mutex_ptr);
                
                index = search_account(accounts,*accounts_no_ptr,
                        request.message.name);
                if(index == -1 || index == *accounts_no_ptr){
                    set_response(&response,CANNOT_START,"Account is not found");
                }else{
                    pthread_mutex_lock(&(accounts[index].account_mutex));
                    if(accounts[index].in_session == IN_SESSION){
                        pthread_mutex_unlock(&(accounts[index].account_mutex));
                        response.code = htonl(CANNOT_START);
                        sprintf(response.message,"Requested account '%s' has been"\
                         " started on another session.",(accounts[index]).name);
                    }else{
                        pthread_mutex_unlock(&(accounts[index].account_mutex));
                        client_account = &accounts[index];
                        account_set_in_session(client_account,IN_SESSION);
                        response.code = htonl(SUCCESS);
                        sprintf(response.message,"Account %s in session.",
                                client_account->name);
                    }
                }
                pthread_mutex_unlock(new_account_lock_mutex_ptr);
                break;
            case EXIT:
                set_response(&response,SUCCESS,"Exit acknowledged.");
                break;
            default:
                set_response(&response,CLIENT_NOT_IN_SESSION,"You must start"\
                        "an account.");
                break;
            }
        // In the customer session
        }else{
            int result = 0;
            float amount = 0.0;
            switch(ntohl(request.code)){
                case OPEN:
                    set_response(&response,CANNOT_OPEN,"Please finish the current"\
                            " session before opening a new account.");
                    break;
                case START:
                    set_response(&response,CANNOT_START,
                            "Please finish the current"\
                            " session before using a new bank account.");
                    break;
                case DEBIT:
                    amount = atof(request.message.amount);
                    account_debit(client_account,amount);
                    set_response(&response,SUCCESS,"");
                    sprintf(response.message,"Successfully debit $%.2f.",
                            amount);
                    break;
                case CREDIT:
                    amount =atof(request.message.amount);
                    result = account_credit(client_account,amount);
                    response.code = result;
                    set_response(&response,result,"");
                    if(result == BALANCE_REACH_ZERO){
                        sprintf(response.message,"Balance reached 0. "\
                                "Your balance is: $%.2f",
                                client_account->balance);
                    }else if( result == SUCCESS){
                        sprintf(response.message,"Successfully credit $%.2f.",
                                amount);
                    }
                    break;
                case BALANCE:
                    set_response(&response,SUCCESS,"");
                    sprintf(response.message,"Account: %s.\nYour balance is %.2f.",
                            client_account->name,client_account->balance);
                    break;
                case EXIT:
                    
                    set_response(&response,SUCCESS,
                            "Exit acknowledged and account session closed.");
                    account_set_in_session(client_account,NOT_IN_SESSION);
                    client_account = NULL; 
                    break;
                case FINISH:
                    account_set_in_session(client_account,NOT_IN_SESSION);
                    set_response(&response,SUCCESS,"Account session closed.");
                    client_account = NULL;
                    break;
               default:
                    response.code = SUCCESS;
                    strcpy(response.message,"Not a valid request.");
                    break;
            }    
        }
        while(write(client_socket_fd, &response, sizeof(response_t))<0){
            printf("Error writting to the client socket %d\n",client_socket_fd);
            continue;
        }
    }  
    // If time_out appears to happen
    if(active_sockets == 0){ 
        if(client_account != NULL){
            account_set_in_session(client_account, NOT_IN_SESSION);
            client_account = NULL;
            response.code = CONNECTION_TIME_OUT;
            strcpy(response.message, "Connection time out.");
            while(write(client_socket_fd, &response, sizeof(response_t))<0){
                printf("Error writting to the client socket %d\n",client_socket_fd);
                continue;
            }    
        }
    }

    close(client_socket_fd);

    // Free the thread index in the thread array
    pthread_mutex_lock(client_thread_arg->thread_check_mutex_ptr);
    memset(client_thread_arg->this_thread_ptr, 0 , sizeof(pthread_t));
    pthread_mutex_unlock(client_thread_arg->thread_check_mutex_ptr);
    
    pthread_exit(NULL);

}
#undef client_socket_fd
#undef accounts
#undef accounts_no_ptr
#undef accounts_max
#undef client_account
#undef new_account_lock_mutex_ptr
