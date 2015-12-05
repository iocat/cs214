#include "account.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
void account_open( account_t* account,char* name){
    pthread_mutex_init(&(account->account_session_mutex),NULL);
    strncpy( account->name, name, NAME_MAX_LENGTH);
    account->balance = 0.0;
    account->in_session = NOT_IN_SESSION;
}
int account_debit(account_t* account, float debit_amount){
    if(account->balance-debit_amount<0){
        return BALANCE_REACH_ZERO;
    }else{
        account->balance-=debit_amount;
        return SUCCESS;
    }
}
void account_credit(account_t* account, float credit_amount){
    account->balance+=credit_amount;
}
float account_balance( account_t* account){
    float balance = account->balance;
    return balance;
}

int account_is_in_session(account_t* account){
    int is_in_session = account->in_session;
    return is_in_session== IN_SESSION;
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
            strcpy(response->message,"Failed to credit: ");
            strcat(response->message,message);
            break;
       case CONNECTION_TIME_OUT:
            strcpy(response->message,"Account timed out."\
                    " Restart your banking applicatio. ");
            break;
       case ACCOUNT_EXIT:
            strcpy(response->message,message);
            break;
   } 
    
}
