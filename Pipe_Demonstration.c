/*******************************************************************************
	Program    : Pipe_Demonstration.c
	Author     : Harry Staley
	Date       : 03/2/2019
	Description: This program demonstrates the capabilities of inter process
 				communicaiton (IPC) through a full duplex unnamed pipe (two pipes)
 				between processes.
 ******************************************************************************/

// Starter code taken from https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/

#include <stdio.h> 	// for printf
#include <unistd.h>	// for fork & pipe stuff.
#include <sys/wait.h> // for wait
#include <string.h> /* for memset */

#define READ 0
#define WRITE 1


int main()
{

	printf("starting IPC...\n");
	printf("Parent and child start doing chores...\n\n");
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
		printf("Child is doing the dishes... We hope...\n\n");

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
				write(childFd[WRITE], "I am doing the dishes.\0", 23);
			} // end for
			sleep(1);
			read(parentFd[READ], childBuf, sizeof(childBuf));
			printf("Child heard: %s\n", childBuf);
			// clear the buffer
			memset(childBuf, 0, sizeof(childBuf));
		} // end for
		
		sleep(5);
		read(parentFd[READ], childBuf, sizeof(childBuf));
		printf("Child heard: %s\n", childBuf);
		memset(childBuf, 0, sizeof(childBuf));
		write(childFd[WRITE], "I am finishing the dishes.\0", 26);
		
		read(parentFd[READ], childBuf, sizeof(childBuf));
		printf("Child heard: %s\n", childBuf);
		memset(childBuf, 0, sizeof(childBuf));		

		sleep(5);		
		// close write connection to parent
		close(childFd[WRITE]);
		sleep(1);
		// close read connection from parent
		close(parentFd[READ]);
	} else {
		printf("Parent is running the vacum cleaner...\n");

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
				printf("Parent heard: %s\n", parentBuf);
				// clear buf
				memset(parentBuf, 0, sizeof(parentBuf));
			}// end for
			
			sleep(1);
			write(parentFd[WRITE], "Are you doing alright?\0", 22);
		}// end for
		
		sleep(5);
		printf("\n55 second simulation ending... Parent calls out.\n");
		write(parentFd[WRITE], "Ok I am done in 5 seconds, you had better be done with the dishes...\0", 68);
		
		sleep(5);
		read(childFd[READ], parentBuf, sizeof(parentBuf));
		printf("Parent heard: %s\n", parentBuf);
		// clear buf
		memset(parentBuf, 0, sizeof(parentBuf));
		
		sleep(5);
		write(parentFd[WRITE], "I am waiting.... Are you finished yet?\0", 38);
		// close write connection to child.
		close(parentFd[WRITE]);
		sleep(1);
		// close read connection from child.
		close(childFd[READ]);
	} // end if

	printf("IPC Finished...\n");
	return 0;
} // end main