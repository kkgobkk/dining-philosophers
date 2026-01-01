/*
src/ipclib.c

author: @kkgobkk
This file implements the functions specified by ../lib/ipclib.h
See that file for more info
*/

#include "../lib/ipclib.h"

/*--------semaphores---------*/

int sem_set_init(int key, int amount)
{
	return semget(key, amount, IPC_CREAT | 0600);
}

int sem_setval(int set_id, int sem_num, int value)
{
	return semctl(set_id, sem_num, sem_num == -1 ? SETALL : SETVAL, value);
}

int sem_wait(int set_id, int sem_num)
{
	struct sembuf sops;
	sops.sem_op = -1;
	sops.sem_num = sem_num;
	sops.sem_flg = 0;

	return semop(set_id, &sops, 1);
}

int sem_signal(int set_id, int sem_num)
{
	struct sembuf sops;
	sops.sem_op = 1;
	sops.sem_num = sem_num;
	sops.sem_flg = 0;

	return semop(set_id, &sops, 1);
}

int sem_wait_for_zero(int set_id, int sem_num){
	struct sembuf sops;
	sops.sem_op = 0;
	sops.sem_num = 0;
	sops.sem_flg = 0;

	return semop(set_id, &sops, 1);
}

int sem_destroy(int set_id)
{
	return semctl(set_id, -1, IPC_RMID);
}
