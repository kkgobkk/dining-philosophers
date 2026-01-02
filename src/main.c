/*
main.c

author: @kkgobkk

this program creates n philosopher processes, n is specified by the first
command line parameter, and handles the end of the simulation, forcing all
children to exit, after SIMULATION_TIME seconds have passed. This program also
creates and manages IPC objects used in the simulation, in particulare a set of n
semaphores used to represent the chopsticks.
The path to the philosopher program can be specified in the second parameter.
If no pathname is specified, the program will look for "bin/philosopher"
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

int sem_synch_id, sem_chopsticks_id;
pid_t* children_pids;
size_t num_children;
time_t start_time = 0, end_time = 0;

/**
 * @brief signal handler for alarms, used to stop the simulation
 * @param sig the signal number
 * @details this signal handler is used to stop the simulation after
 * SIMULATION_TIME seconds have passed (SIGALRM), or after this process has been
 * terminated (SIGINT or SIGTERM). This handler terminates all philosophers.
 */
void signal_handler(int signum);

/**
 * @brief release resources, destroy IPC object, print statistics
 */
void cleanup(void);

int main(int argc, char** argv){
	char* child_argv[4] = {"\0", argv[1], (char*)malloc(21), NULL};
	char* philosopher_path;
	int child_exit_status;
	struct sigaction signal_action;

	/*-------get number of children and path from args--------*/
	if (argc < 2)
	{
		fprintf(stderr, "error: wrong number of parameters (must have 1 or 2)\n");
		exit(EXIT_FAILURE);
	}
	else if (argc == 2)
	{
		philosopher_path = "bin/philosopher";
	}
	else
	{
		philosopher_path = argv[2];
	}

	num_children = strtoul(argv[1], NULL, 10);
	handle_errors();
	children_pids = (pid_t*)malloc(sizeof(pid_t) * num_children);
	handle_errors();

	
	/*-----set up signal handlers--------*/
	signal_action.sa_handler = signal_handler;
	sigemptyset(&signal_action.sa_mask);
	signal_action.sa_flags = 0;
	sigaction(SIGALRM, &signal_action, NULL);
	handle_errors();
	sigaction(SIGTERM, &signal_action, NULL);
	handle_errors();
	sigaction(SIGINT, &signal_action, NULL);
	atexit(cleanup);

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
		snprintf(child_argv[2], 21, "%zu", i);	//pass each philopher's index to it
		switch (tmp = fork())
		{
			case 0:
				execv(philosopher_path, child_argv);
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
	printf("\nsimulation started\n\n");
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

	exit(EXIT_SUCCESS);
}

void signal_handler(int signum)
{
	int child_exit_status = 0;

	/*------kill all children------*/
	for (size_t i = 0; i < num_children; i++)
	{
		kill(children_pids[i], SIGTERM);
		handle_errors();
	}

	/*-------wait for all children to exit---------*/
	while (wait(&child_exit_status) != -1)
	{
		if (errno != 10) {
			handle_errors();
		}
	}
	errno = 0;

	exit(EXIT_SUCCESS);
}

void cleanup()
{
	/*-------end the simulation and free resources and IPC objects---------*/
	end_time = time(NULL);
	printf("\nsimulation ended after %ld seconds\n", start_time != 0 ? end_time - start_time : 0);
	
	free(children_pids);

	sem_destroy(sem_synch_id);
	handle_errors();
	sem_destroy(sem_chopsticks_id);
	handle_errors();
}
