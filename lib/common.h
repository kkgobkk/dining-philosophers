/*
lib/common.c

author: @kkgobkk
This file provides useful macros that are used across all src files
*/


#ifndef LIB_COMMON
#define LIB_COMMON

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*--------ERROR HANDLING---------*/

//prints a description of the error to stderr, then exits using the error number as an exit code
#define handle_errors() if(errno){fprintf(stderr, "error %d (%s) in process %d (file %s, line %d)\n", errno, strerror(errno), getpid(), __FILE__, __LINE__); exit(errno);}

/*-------IPC OBJECTS KEYS--------*/
#define SEM_SYNCH_KEY 7282639
#define SEM_CHOPSTICKS_KEY 5287302

#endif
