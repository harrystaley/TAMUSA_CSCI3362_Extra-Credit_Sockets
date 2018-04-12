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
#include <string.h> /* for memset */

#define READ 0
#define WRITE 1


int main()
{

	printf("start...\n");
	printf("Mother and child enter a grocery store...\n\n");
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
		printf("Child is on his own... possibly in toys...\n\n");

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
			// clear the buffer
			memset(childBuf, 0, sizeof(childBuf));
		}
		
		sleep(5);
		read(parentFd[READ], childBuf, sizeof(childBuf));
		printf("Child recieved: %s\n", childBuf);
		memset(childBuf, 0, sizeof(childBuf));
		write(childFd[WRITE], "I am coming mom.\0", 16);
		
		read(parentFd[READ], childBuf, sizeof(childBuf));
		printf("Child recieved: %s\n", childBuf);
		memset(childBuf, 0, sizeof(childBuf));		

		sleep(5);		
		// close write connection to parent
		close(childFd[WRITE]);
		sleep(1);
		// close read connection from parent
		close(parentFd[READ]);
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
				// clear buf
				memset(parentBuf, 0, sizeof(parentBuf));
			}// end for
			
			sleep(1);
			write(parentFd[WRITE], "Are you doing alright?\0", 22);
		}// end for
		
		sleep(5);
		printf("\n55 second simulation completing... mom sends a message.\n");
		write(parentFd[WRITE], "Ok I am done in 5 seconds, now come to the main door...\0", 55);
		
		sleep(5);
		read(childFd[READ], parentBuf, sizeof(parentBuf));
		printf("Mom recieved: %s\n", parentBuf);
		// clear buf
		memset(parentBuf, 0, sizeof(parentBuf));
		
		sleep(5);
		write(parentFd[WRITE], "I am waiting.... come on...\0", 27);
		// close write connection to child.
		close(parentFd[WRITE]);
		sleep(1);
		// close read connection from child.
		close(childFd[READ]);
	} // end if
	
	printf("Finished...\n");
	return 0;
} // end main

