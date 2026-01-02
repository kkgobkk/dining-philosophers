/*
philosopher-asymm.c

author: @kkgobkk
This program indefinitely alternates between "eating" and "thinking". It can
switch from eating to thinking whenver it wants, but to do the opposite it needs
to acquire two chopsticks, specifically the chopsticks of index i, and (i+1)%N.
N is the number of processes of this type that exist in the simulation.
This version is "asymmetric": depending on the parity of the index of the philopher, 
it will try to pick up the left or right chopstick first. This ensures that no
deadlock is possible
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../lib/common.h"
#include "../lib/ipclib.h"

size_t	my_index; //the index of this philosopher

/**
 * @brief this function is used to handle SIGTERM and SIGINT signals
 * @param signum the value of the received signal
 * @details used to ensure "graceful" termination and printing
 * on screen so that the user knows the process actually terminated
 */
void sigterm_handler(int signum);

int main(int argc, char** argv)
{
	pid_t my_pid = getpid();
	size_t philosophers_num, left_chopstick, right_chopstick;
	int sem_synch_id, sem_chopsticks_id;
	struct sigaction sigterm_action;

	/*------initialization-------*/
	srand(time(NULL) + my_pid);
	philosophers_num = strtoul(argv[1], NULL, 10);
	handle_errors();
	my_index = strtoul(argv[2], NULL, 10);
	handle_errors();

	sem_synch_id = sem_set_init(SEM_SYNCH_KEY, 1);
	handle_errors();
	sem_chopsticks_id = sem_set_init(SEM_CHOPSTICKS_KEY, philosophers_num);
	handle_errors();

	left_chopstick = my_index;
	right_chopstick = (my_index + 1) % philosophers_num;

	sigterm_action.sa_handler = sigterm_handler;
	sigemptyset(&sigterm_action.sa_mask);
	sigterm_action.sa_flags = 0;
	sigaction(SIGTERM, &sigterm_action, NULL);
	handle_errors();
	sigaction(SIGINT, &sigterm_action, NULL);


	/*-------synchronize start of the cycle-------*/
	printf("philosopher %zu (pid %d) ready\n", my_index, my_pid);
	sem_wait(sem_synch_id, 0);
	handle_errors();
	sem_wait_for_zero(sem_synch_id, 0);
	handle_errors();
	sem_wait(sem_synch_id, 0);

	/*------main cycle-------*/
	if (my_index % 2 == 0)
	{
		while (1)
		{
			sem_wait(sem_chopsticks_id, left_chopstick);
			printf("philosopher %zu (pid %d) picked up chopstick %zu (left)\n", my_index, my_pid, left_chopstick);
			//sleep(1);
			sem_wait(sem_chopsticks_id, right_chopstick);
			printf("philosopher %zu (pid %d) picked up chopstick %zu (right)\n", my_index, my_pid, right_chopstick);
		
			printf("philosopher %zu (pid %d) is eating\n", my_index, my_pid);
			sleep(5 + rand() % 6);

			sem_signal(sem_chopsticks_id, left_chopstick);
			sem_signal(sem_chopsticks_id, right_chopstick);
			printf("philosopher %zu (pid %d) is thinking\n", my_index, my_pid);
		}
	}
	else
	{
		while (1)
		{
			sem_wait(sem_chopsticks_id, right_chopstick);
			printf("philosopher %zu (pid %d) picked up chopstick %zu (right)\n", my_index, my_pid, right_chopstick);
			//sleep(1);
			sem_wait(sem_chopsticks_id, left_chopstick);
			printf("philosopher %zu (pid %d) picked up chopstick %zu (left)\n", my_index, my_pid, left_chopstick);
		

			printf("philosopher %zu (pid %d) is eating\n", my_index, my_pid);
			sleep(5 + rand() % 6);

			sem_signal(sem_chopsticks_id, left_chopstick);
			sem_signal(sem_chopsticks_id, right_chopstick);
			printf("philosopher %zu (pid %d) is thinking\n", my_index, my_pid);
		}
	}

	exit(EXIT_SUCCESS);
}

void sigterm_handler(int sig)
{
	printf("philosopher %zu (pid %d) exiting\n", my_index, getpid());
	exit(EXIT_SUCCESS);
}
