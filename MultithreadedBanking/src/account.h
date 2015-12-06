#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <pthread.h>
#define NAME_MAX_LENGTH 101
#define REQUEST_MES_LENGTH 101
#define RESPONSE_MES_LENGTH 300
#define AMOUNT_SIZE 100
#define MAX_ACCOUNT 20
typedef struct account_t{
    char name[NAME_MAX_LENGTH];
    float balance;
    // Session mutex lock
    pthread_mutex_t account_session_mutex;
    enum {NOT_IN_SESSION=0,IN_SESSION} in_session;
} account_t;

void account_open( account_t* account,char* name);
int account_debit(account_t* account, float credit_amount);
void account_credit(account_t* account, float debit_amount);
float account_balance( account_t* account);
int account_is_in_session(account_t* account);

#endif
