#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <pthread.h>

/* An account structure to be stored online */
typedef struct account_t{
    char name[100];
    float balance;
    pthread_mutex_t* account_mutex_ptr;
    enum {IN_SESSION=0,NOT_IN_SESSION} in_session;
} account_t;

#endif
