#include "account.h"
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
