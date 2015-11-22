#include "server-session.h"
void * session_subroutine(void* arg){
    session_thread_info_t* session_thread_info = (session_thread_info_t*) arg;
    // Do stuffs     

    pthread_exit(NULL);
}
