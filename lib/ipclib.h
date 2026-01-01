/*
lib/ipclib.h

author: @kkgobkk

This file provides a few wrapper functions to use systemV IPC objects
with an easier interface.
*/


#ifndef LIB_IPCLIB
#define LIB_IPCLIB

#include <sys/sem.h>

/**
 * @brief init ializes a set of semaphores
 * @param key the key used to identifies the semaphore
 * @param amount the amount of semaphores in the set
 * @return the id of the semaphore set
 * @details must be called before any other function working
 * on semaphores. Every process that must share the same semaphore
 * must use the same value for the parameter "key"
 */
int sem_set_init(int key, int amount);

/**
 * @brief sets a semaphore (or set of semaphores) to a certain value
 * @param set_id id of the set of semaphores
 * @param sem_num index of the single semaphore within the set. Use -1 for the whole set
 * @param value the value to set the semaphore(s) to
 * @return -1 if there were any errors
 */
int sem_setval(int set_id, int sem_num, int value);


/**
 * @brief blocks the process until the sem is positive, then decreases its value by 1
 * @param set_id id of the set of semaphores
 * @param sem_num index of the single semaphore within the set (-1 for the whole set)
 * @return -1 if there were any errors
 */
int sem_wait(int set_id, int sem_num);

/**
 * @brief increases the value of a semaphore by 1
 * @param set_id id of the set of semaphores
 * @param sem_num index of the single semaphore within the set (-1 for the whole set)
 * @return -1 if there were any errors
 */
int sem_signal(int set_id, int sem_num);

/**
 * @brief blocks the process until a semaphore is equal to 0
 * @param set_id id of the set of semaphores
 * @param sem_num index of the single semaphore within the set (-1 for the whole set)
 * @return -1 if there were any errors
 */
int sem_wait_for_zero(int set_id, int sem_num);

/**
 * @brief deletes a semaphore set
 * @param set_id id of the set of semaphores
 * @return -1 if there were errors
 */
int sem_destroy(int set_id);

#endif
