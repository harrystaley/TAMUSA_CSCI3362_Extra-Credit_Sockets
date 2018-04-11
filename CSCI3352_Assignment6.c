/*******************************************************************************
 * Program    : CSCI3352_Assignment5.c
 * Author     : Harry Staley
 * Date       : 03/28/2018
 * Description: Assignment 6 of Systems Programming
 ******************************************************************************/

/* Includes */
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
/*
 Use fork(), pipe(), sleep() system calls in this program to tell the child and parent to respond.
 Starter code taken from Ch 10 Slide 15, 16, and 37

*/
 /* In alarm.c, the first function, ding, simulates an alarm clock. */
static int alarm_fired = 0;

void mysig(int sig)
{
	int status;
	pid_t pid;
	printf("Signal %d \n", sig);
	if (sig == SIGALRM) /*System alarm */
	{
		alarm_fired = 1;
	} // end if
	
	if (sig == SIGCLD) /*One of Children processes has terminated */
	{
		/* harvest terminated DEFUNCT child process */
		pid = waitpid(-1, &status, WNOHANG);
		/*return immediately if no child has exited */
		printf(" Child Process(%d) terminated with a status of %d\n", pid, status);
	} // end if
} // end mysig

/* In main, we tell the child process to wait for five seconds before sending a SIGALRM signal to its parent. */
int main()
{
	int pid;
	printf("alarm application starting\n");
	if((pid = fork()) == 0)
	{
		/* Child process */
		sleep(10);
		/* Wake up parent process with a SIGALRM */
		kill(getppid(), SIGALRM);
		exit(0);
	} // end if
	/*The parent process arranges to catch SIGALRM with a call to signal and then waits for the inevitable. */
	printf("waiting for alarm to go off\n");
	(void) signal(SIGALRM, mysig);
	(void) signal(SIGCLD, mysig);
	do {
		pause(); /* Wait for any signal call */
	} while(!alarm_fired);
	printf("Ding!\007\n");
	printf("done\n");
	exit(0);
} // end main

// Starter code from Slide 37

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1
int main()
{
	// for printf
	// for fork & pipe stuff.
	// for wait
	printf("Parent running\n");
	// Create the pipe before the fork so that the child // process has a copy of it after the fork.
	int fd[2];
	if (pipe(fd) == -1)
	{
		printf("Pipe creation failed\n");
		return -1;
	} // end if

	// Create child process.
	int pid = fork();
	// fork returns pid of child process in parent // and 0 in the child.
	if (pid != 0)
	{
		printf("Parent running after fork\n");
		// The parent will write to the pipe. So close
		// the read end of the pipe and begin writing data. close(fd[READ_END]);
		char *str1 = "printing to the pipe.\0";
		char *str2 = "putting more in pipe.\0";
		printf("Parent writing: %s\n", str1);
		write(fd[WRITE_END], str1, 21);
		sleep(3);
		printf("Parent writing: %s\n", str2); write(fd[WRITE_END], str2, 21);
	    close(fd[WRITE_END]);
	    printf("Parent done\n");
	} else {
	 	printf("Child running\n");
		// The child will read from the pipe. So close the // write end of the pipe and begin reading until the // parent closes the pipe.
		close(fd[WRITE_END]);
		char buf[1];
		while(read(fd[READ_END], buf, 1) != 0)
		{
	    	printf("%s\n",buf);
	  	} // end while
	    close(fd[READ_END]);
	    printf("Child done\n");
	} // end if
} // end main