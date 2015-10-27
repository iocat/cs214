#ifndef READ_H
#define READ_H

#include "index.h"

/*
 *  read_file() takes in a relative path of the file
 *  Read a file and store its data into the sorted list of tokens prototyped 
 *  as in index.h
 */
char* read_file(char* path);
#endif
