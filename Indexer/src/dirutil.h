#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

char* read_file(const char * const);
int is_regular_file(const char*);
int is_dir(const char*);

