#include "reqres.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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
