#ifndef _REQRES_H_
#define _REQRES_H_
#include <pthread.h>
#define NAME_MAX_LENGTH 101
#define REQUEST_MES_LENGTH 101
#define RESPONSE_MES_LENGTH 300
#define AMOUNT_SIZE 100
enum response_error_code_t{
        SUCCESS,
        CANNOT_OPEN,
        CANNOT_START,
        CLIENT_NOT_IN_SESSION,
        ACCOUNT_UNFOUND,
        BALANCE_REACH_ZERO,
        CONNECTION_TIME_OUT,
        ACCOUNT_EXIT
};
typedef struct response_t{
    int code;
    char message[RESPONSE_MES_LENGTH];
} response_t;

// Read and write request code as character!
enum client_request_code_t{
        OPEN ,
        START ,
        CREDIT,
        DEBIT,
        BALANCE ,
        FINISH ,
        EXIT
};
typedef struct request_t{
    int code;  
    union {
        char name[NAME_MAX_LENGTH];
        char amount[AMOUNT_SIZE];
    } message;
} request_t;
// listen_request reads incoming request from the socket fd
int listen_request(request_t* request, int socket_fd);
void form_response(response_t* response, int code, char* message);


#endif
