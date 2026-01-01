/*
main.c

author: @kkgobkk

this program creates n philosopher processes, n is specified by the first
and only command line parameter, and handles the end of the simulation, forcing all
children to exit, after SIMULATION_TIME seconds have passed. This program also
creates and manages IPC objects used in the simulation, in particulare a set of n
semaphores used to represent the chopsticks.
*/

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

#include "../lib/common.h"
#include "../lib/ipclib.h"

#define SIMULATION_TIME 30

pid_t* children_pids;
size_t num_children;

/**
 * @brief signal handler for alarms, used to stop the simulation
 * @param sig the signal number
 * @details this signal handler is used to stop the simulation after
 * SIMULATION_TIME seconds have passed. All philosophers are terminated.
 */
void alarm_handler(int sig);

int main(int argc, char** argv){
	int child_exit_status, sem_synch_id, sem_chopsticks_id;
	char* child_argv[4] = {argv[0], argv[1], (char*)malloc(21), NULL};
	time_t start_time, end_time;
	struct sigaction alarm_action;

	/*-------get number of children from args--------*/
	if (argc != 2)
	{
		fprintf(stderr, "error: wrong number of parameters (must have 1)\n");
		exit(EXIT_FAILURE);
	}

	num_children = strtoul(argv[1], NULL, 10);
	handle_errors();
	children_pids = (pid_t*)malloc(sizeof(pid_t) * num_children);
	handle_errors();

	/*-----set up signal handler for alarm--------*/
	alarm_action.sa_handler = alarm_handler;
	sigemptyset(&alarm_action.sa_mask);
	alarm_action.sa_flags = 0;
	sigaction(SIGALRM, &alarm_action, NULL);
	handle_errors();

	/*------create and init IPC objects------*/
	sem_synch_id = sem_set_init(SEM_SYNCH_KEY, 1);
	handle_errors();
	sem_setval(sem_synch_id, 0, num_children);
	handle_errors();

	sem_chopsticks_id = sem_set_init(SEM_CHOPSTICKS_KEY, num_children);
	handle_errors();
	for(size_t i = 0; i < num_children; i ++)
	{
		sem_setval(sem_chopsticks_id, i, 1);
	}
	handle_errors();

	/*-----create and execute children--------*/
	printf("creating %ld philosopher processes\n\n", num_children);
	pid_t tmp;
	for (size_t i = 0; i < num_children; i++)
	{
		snprintf(child_argv[2], 21, "%zu", i);
		switch (tmp = fork())
		{
			case 0:
				execv("bin/philosopher", child_argv);
				handle_errors();
				break;

			case -1:
				handle_errors();
			
			default:
				children_pids[i] = tmp;
		}
	}
	
	/*--------start the simulation---------*/
	sem_wait_for_zero(sem_synch_id, 0);
	printf("simulation started\n");
	sem_setval(sem_synch_id, 0, num_children);
	start_time = time(NULL);
	alarm(SIMULATION_TIME);

	/*-------wait for all children to exit---------*/
	while (wait(&child_exit_status) != -1)
	{
		if (errno != 10) {
			handle_errors();
		}
	}
	errno = 0;

	/*-------end the simulation, free resources and IPC objects, then exit---------*/
	end_time = time(NULL);
	printf("\nsimulation ended after %ld seconds\n", end_time - start_time);
	
	free(children_pids);

	sem_destroy(sem_synch_id);
	handle_errors();
	sem_destroy(sem_chopsticks_id);
	handle_errors();

	exit(EXIT_SUCCESS);
}

void alarm_handler(int sig)
{
	for (size_t i = 0; i < num_children; i++)
	{
		kill(children_pids[i], SIGTERM);
		handle_errors();
	}
}
