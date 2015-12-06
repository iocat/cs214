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
        return 0;
    }else{
        account->balance-=debit_amount;
        return 1;
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
    return account-> in_session == IN_SESSION;
}

