/*******************************************************************************
	Program    : Server.c
	Author     : Harry Staley
	Date       : 03/2/2019
	Description: This program demonstrates the capabilities of sockets
                 from a server to a client.
 ******************************************************************************/

// Starter code taken from https://www.geeksforgeeks.org/socket-programming-cc/
// With further research from https://www.thegeekstuff.com/2011/12/c-socket-programming/?utm_source=feedburner

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

#define PORT 5000 // Defines the port that will be used.

int main(int argc, char const *argv[])
{
    struct sockaddr_in sockAddrIn;
    int socketFd; // server socket file descriptor
    int queLen = 3; // the number of connections slots in the listen que.
    int connSocketFd; // new socket file descriptor
    int optEnabled = 1;
    int addressLen = sizeof(sockAddrIn);
    char buf[1024] = {0};
    char *serverMsg;
    time_t ticks = time(NULL);

    /*
     * CREATE SOCKET FILE DESCRIPTOR
     *
     * Create a socket file descriptor as defined by:
     * AF_INET = IPv4
     * SOCK_STREAM = TCP
     * 0 = Defines IP Protocol as it appears on protocol field in the IP header of a packet.
     */

    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }// end if socket

    //printf("SERVER: socket FD created\n");

    /*
     * SET SOCKET OPTIONS
     *
     * This helps in manipulating options for the socket referred by the file descriptor socketFd and the individual
     * option is defined by the third parameter and the status is set by optEnabled. This is completely optional,
     * but it helps in reuse of address and port. Prevents error such as: “address already in use”.
     */

    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optEnabled, sizeof(optEnabled)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }// end if setsockopt

    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_addr.s_addr = INADDR_ANY;
    sockAddrIn.sin_port = htons( PORT );

    //printf("SERVER: Socket Options Set\n");

    /*
     * BIND THE SOCKET
     *
     * The bind() function binds the socket to the address and port number specified in sockAddrIn. in this project, the
     * server is bound to localhost which is why we use INADDR_ANY to refer to the IP address.
     */

    if (bind(socketFd, (struct sockaddr *)&sockAddrIn, sizeof(sockAddrIn))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }// end if bind

    //printf("SERVER: Bound on port: %i\n", PORT);

    /*
     * LISTEN FOR A CONNECTION
     *
     * The listen() system call tells the server socket to wait for the client to approach the server to make
     * a connection. The backlog, defines the maximum length to which the queue of pending connections for socketFd
     * may grow. If a connection request arrives when the queue is full, the client may receive an error with an
     * indication of ECONNREFUSED.
     */

    if (listen(socketFd, queLen) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }// end if listen
    printf("\n%.24s SERVER: Listening on port: %i\n", ctime(&ticks), PORT);


    /*
     * ACCEPT THE CONNECTION
     *
     * The accept() system call extracts the first connection request on the queue of pending connections for the listening socket and socket file descriptor.
     * Then creates a new connected socket, and returns a new file descriptor referring to that socket.
    */

     if ((connSocketFd = accept(socketFd, (struct sockaddr *)&sockAddrIn, (socklen_t*)&addressLen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }// end if connSocketFd

    //printf("SERVER: Connection Accepted\n");

    // CONNECTION ESTABLISHED W. CLIENT!!!!!

    recv( connSocketFd , buf, 1024, 0);
    printf("%.24s SERVER: %s\n", ctime(&ticks),buf );

    serverMsg = "Hello from server";
    send(connSocketFd , serverMsg , strlen(serverMsg) , 0 );
    printf("%.24s SERVER: Hello message sent\n", ctime(&ticks));

    sleep(1); // Wait before closing the file descriptors.
    close(socketFd);
    sleep(1);
    close(connSocketFd);


    return 0;
}// end method main
