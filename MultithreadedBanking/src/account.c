#include "account.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
void account_open( account_t* account,char* name){
    pthread_mutex_init(&(account->account_mutex),NULL);
    pthread_mutex_lock(&(account->account_mutex));
    strncpy( account->name, name, NAME_MAX_LENGTH);
    account->balance = 0.0;
    account->in_session = NOT_IN_SESSION;
    pthread_mutex_unlock(&(account->account_mutex));
}
int account_debit(account_t* account, float debit_amount){
    pthread_mutex_lock(&(account->account_mutex));
    if(account->balance-debit_amount<0){
        pthread_mutex_unlock(&(account->account_mutex));
        return BALANCE_REACH_ZERO;
    }else{
        account->balance-=debit_amount;
        pthread_mutex_unlock(&(account->account_mutex));
        return SUCCESS;
    }
}
void account_credit(account_t* account, float credit_amount){
    pthread_mutex_lock(&(account->account_mutex));
    account->balance+=credit_amount;
    pthread_mutex_unlock(&(account->account_mutex));
}
float account_balance( account_t* account){
    pthread_mutex_lock(&(account->account_mutex));
    float balance = account->balance;
    pthread_mutex_unlock(&(account->account_mutex));
    return balance;
}

int account_is_in_session(account_t* account){
    pthread_mutex_lock(&(account->account_mutex));
    int is_in_session = account->in_session;
    pthread_mutex_unlock(&(account->account_mutex));
    return is_in_session== IN_SESSION;
}
void account_set_in_session(account_t* account, int session_code){
    pthread_mutex_lock(&(account->account_mutex));
    account->in_session = session_code;
    pthread_mutex_unlock(&(account->account_mutex));
}
int listen_request(request_t* request, int socket_fd){
    int byte_read = read(socket_fd, request,sizeof(request_t));
    request->code = ntohl(request->code);
    return byte_read;
}
void form_response(response_t* response,int code, char* message){
   response->code = htonl(code);
   switch(code){
       case SUCCESS:
            strcpy(response->message,message);
            break;
       case CANNOT_OPEN:
            strcpy(response->message,"Cannot open the account: ");
            strcat(response->message,message);
            break;
       case CANNOT_START:
            strcpy(response->message,"Cannot start the session: ");
            strcat(response->message,message);
            break;
       case CLIENT_NOT_IN_SESSION:
            strcpy(response->message,"Please start an account session with start: ");
            break;
       case ACCOUNT_UNFOUND:
            strcpy(response->message,"Cannot find account named : ");
            strcat(response->message,message);
            break;
       case BALANCE_REACH_ZERO:
            strcpy(response->message,"Your balance reached 0. Failed to credit."\
                    " Your balance is: ");
            strcat(response->message,message);
            break;
       case CONNECTION_TIME_OUT:
            strcpy(response->message,"Account timed out."\
                    " Restart your banking application.");
            break;
       case ACCOUNT_EXIT:
            strcpy(response->message,message);
            break;
   } 
    
}
