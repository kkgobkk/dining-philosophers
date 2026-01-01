/*
philosopher.c

author: @kkgobkk
This program indefinitely alternates between "eating" and "thinking". It can
switch from eating to thinking whenver it wants, but to do the opposite it needs
to acquire two chopsticks, specifically the chopsticks of index i, and (i+1)%N.
N is the number of processes of this type that exist in the simulation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../lib/common.h"
#include "../lib/ipclib.h"

int main(int argc, char** argv)
{
	pid_t my_pid = getpid();
	size_t index, //the index of this philosopher
				 philosophers_num, left_chopstick, right_chopstick;
	int sem_synch_id, sem_chopsticks_id;

	/*------initialization-------*/
	srand(time(NULL) + my_pid);
	philosophers_num = strtoul(argv[1], NULL, 10);
	handle_errors();
	index = strtoul(argv[2], NULL, 10);
	handle_errors();

	sem_synch_id = sem_set_init(SEM_SYNCH_KEY, 1);
	handle_errors();
	sem_chopsticks_id = sem_set_init(SEM_CHOPSTICKS_KEY, philosophers_num);
	handle_errors();

	left_chopstick = index;
	right_chopstick = (index + 1) % philosophers_num;

	/*-------synchronize start of the cycle-------*/
	printf("philosopher %zu (pid %d) ready\n", index, my_pid);
	sem_wait(sem_synch_id, 0);
	handle_errors();
	sem_wait_for_zero(sem_synch_id, 0);
	handle_errors();
	sem_wait(sem_synch_id, 0);

	/*------main cycle-------*/
	while (1)
	{
		sem_wait(sem_chopsticks_id, left_chopstick);
		sem_wait(sem_chopsticks_id, right_chopstick);
		
		printf("philosopher %zu (pid %d) is eating\n", index, my_pid);
		sleep(5 + rand() % 6);

		sem_signal(sem_chopsticks_id, left_chopstick);
		sem_signal(sem_chopsticks_id, right_chopstick);
		printf("philosopher %zu (pid %d) is thinking\n", index, my_pid);
	}

	exit(EXIT_SUCCESS);
}
