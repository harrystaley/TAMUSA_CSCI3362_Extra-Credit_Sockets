/*******************************************************************************
 * Program    : CSCI3352_Assignment5.c
 * Author     : Harry Staley
 * Date       : 03/28/2018
 * Description: Assignment 6 of Systems Programming
 ******************************************************************************/

// Starter code from Ch 10 Slide 37

#include <stdio.h> 	// for printf
#include <unistd.h>	// for fork & pipe stuff.
#include <sys/wait.h> // for wait
#include <math.h> // to round the count

#define READ 0
#define WRITE 1

int main()
{

	printf("start...\n");
	printf("Mother and child enter a grocery store...\n");
	// Create the pipe before the fork so that the child
	// process has a copy of it after the fork.
	int parentFd[2];
	int childFd[2];

	if (pipe(childFd) == -1)
	{
		printf("childSendFd Pipe creation failed\n");
		return -1;
	} // end if
	
	if (pipe(parentFd) == -1)
	{
		printf("parentSendFd Pipe creation failed\n");
		return -1;
	} // end if

	// Create child process.
	int pid = fork();
	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}

	// fork returns pid of child process in parent
	// and 0 in the child.
	if (pid == 0)
	{
		printf("Child process with id = %d is created\n", pid);
		printf("Child is on his own... possibly in toys...\n");

		close(parentFd[WRITE]); // closes the write end of the parent pipe
		close(childFd[READ]); // closes the read end of the child pipe

		// The child will read from the pipe. So close the
		// write end of the pipe and begin reading until the
		// parent closes the pipe.

		char childBuf[100];		
		int i;
		for ( i = 0; i < 5; i++)
		{	
			int j; 
			for ( j = 0; j < 3; j++)
			{
				sleep(3);
				write(childFd[WRITE], "I am doing alright mom.\0", 23);
			}
			sleep(1);
			read(parentFd[READ], childBuf, sizeof(childBuf));
			printf("Child recieved: %s\n", childBuf);			
			
		}
		sleep(5);
		write(childFd[WRITE], "I am coming mom.\0", 16);
		
		read(parentFd[READ], childBuf, sizeof(childBuf));
		printf("Child recieved: %s\n", childBuf);
		
		close(childFd[WRITE]);

	} else {
		printf("Mother is now doing shopping...\n");

		close(childFd[WRITE]); // closes the read end of the parent pipe
		close(parentFd[READ]); // closes the read end of the child pipe

		char parentBuf[100];
		int i;
		for ( i = 0; i < 5; i++)
		{
			int j;
			for ( j = 0; j < 3; j++)
			{
				sleep(3);
				read(childFd[READ], parentBuf, sizeof(parentBuf));
				printf("Mom recieved: %s\n", parentBuf);
			}// end for
			sleep(1);
			write(parentFd[WRITE], "Are you doing alright?\0", 22);
		}// end for
		sleep(5);
		printf("55 second simulation completing... mom sends a message\n");
		write(parentFd[WRITE], "Ok I am done in 5 seconds.\0", 26);
		read(childFd[READ], parentBuf, sizeof(parentBuf));
		printf("Mom recieved: %s\n", parentBuf);
		sleep(5);
		printf("I am waiting.... come on...\n");
		close(parentFd[WRITE]);

	} // end if
} // end main

