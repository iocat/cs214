#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

/* 
 * regular_file_to_string reads the given file from path to a memory allocated string 
 * The given file needed to be a regular file. Otherwise, NULL will be returned.
 * regular_file_to_string() returns a buffer of characters read from the file
 * Users of this function are responsible for freeing the string
 */
char* regular_file_to_string(const char * const);
/*
 * is_regular_file() checks the given path to see whether that path points to a regular file or not 
 */
int is_regular_file(const char* const);
/*
 *  is_dir() checks the given path to see whether it is a directory or not
 */
int is_dir(const char*const);
/*
 * exists() shows whether the given path points to a file or directory
 */
int exists(const char * const);
