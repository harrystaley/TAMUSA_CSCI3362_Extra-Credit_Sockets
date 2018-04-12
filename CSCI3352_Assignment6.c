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

#define READ_END 0
#define WRITE_END 1

int main()
{

	printf("start...\n");
	printf("Mother and child enter a grocery store...\n");
	// Create the pipe before the fork so that the child
	// process has a copy of it after the fork.
	int parentSendFd[2];
	int childSendFd[2];

	if (pipe(childSendFd) == -1)
	{
		printf("childSendFd Pipe creation failed\n");
		return -1;
	} // end if
	
	if (pipe(parentSendFd) == -1)
	{
		printf("parentSendFd Pipe creation failed\n");
		return -1;
	} // end if

	// Create child process.
	int pid = fork();

	// fork returns pid of child process in parent
	// and 0 in the child.
	if (pid == 0)
	{
		printf("Child process with id = %d is created\n", pid);
		printf("Child is on his own... possibly in toys...\n");

		close(parentSendFd[WRITE_END]); // closes the wrote end of the parent pipe
		close(childSendFd[READ_END]); // closes the read end of the child pipe

		// The child will read from the pipe. So close the
		// write end of the pipe and begin reading until the
		// parent closes the pipe.
		int j;
		for ( j = 0; j < 15; ++j)
		{
			//sleep(3);
			write(childSendFd[WRITE_END], "Mom recieved: I am doing alright.\n\0", 30);
		}
		//sleep(5);
		write(childSendFd[WRITE_END], "Mom recieved: I am coming mom.\n\0", 30);
		close(childSendFd[WRITE_END]);
	
		char childBuf[1];
		while(read(parentSendFd[READ_END], childBuf, 1) != 0)
		{
			printf("%s", childBuf);
		} // end while
		close(parentSendFd[READ_END]);

	}  
	
	if (pid != 0)
	{
		printf("Mother is now doing shopping...\n");

		close(childSendFd[WRITE_END]); // closes the read end of the parent pipe
		close(parentSendFd[READ_END]); // closes the read end of the child pipe

		int i;
		for ( i = 0; i < 5; ++i)
		{
			//sleep(10);
			write(parentSendFd[WRITE_END], "Child recieved: Are you doing alright?\n\0", 39);
		}
		//sleep(5);
		printf("55 second simulation completing... mom sends a message\n");
		write(parentSendFd[WRITE_END], "Child recieved: Ok I am done if 5 seconds.\n\0", 43);
		//sleep(5);
		printf("I am waiting.... come on...\n");
		close(parentSendFd[WRITE_END]);

		char parentBuf[1];
		while(read(childSendFd[READ_END], parentBuf, 1) != 0)
		{
			printf("%s", parentBuf);
		} // end while
		close(childSendFd[READ_END]); // closes the read end of the pipe

	} // end if
} // end main

