/*******************************************************************************
	Program    : Client.c
	Author     : Harry Staley
	Date       : 03/2/2019
	Description: This program demonstrates the capabilities of communicaiton
                 through sockets from a client to a server.
 ******************************************************************************/

// Starter code taken from https://www.geeksforgeeks.org/socket-programming-cc/
// With further research from https://www.thegeekstuff.com/2011/12/c-socket-programming/?utm_source=feedburner

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define IP_ADDRESS "127.0.0.1" // Defines the IP Address for the local host
#define PORT 5000 // Defines the port that will be used.

int main(int argc, char const *argv[])
{
    int socketFd = 0; // socket file descriptor
    struct sockaddr_in serverAddr;
    char *clientMsg;
    char buf[1024] = {0};
    time_t ticks = time(NULL);

    sleep(1); // Wait for one second before trying to connect to the server.

    /*
     * CREATE SOCKET FILE DESCRIPTOR
     *
     * Create a socket file descriptor as defined by:
     * AF_INET = IPv4
     * SOCK_STREAM = TCP
     * 0 = Defines IP Protocol as it appears on protocol field in the IP header of a packet.
     */

    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error on client\n");
        return -1;
    }// end if socket

    //printf("CLIENT: socket created\n");

    memset(&serverAddr, '0', sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // Convert IPv4 or IPv6 addresses from text to binary
    if(inet_pton(AF_INET, IP_ADDRESS, &serverAddr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }// end if inet_ption

    //printf("CLIENT: converted %s to binary.\n", IP_ADDRESS);

    /*
    * CONNECT TO THE SERVER
     *
     * The connect() system call connects the socket referred to by the file descriptor socketFd to the address
     * specified by serverAddr where the server’s address and port is specified in serverAddr.
     */

    if (connect(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("\nConnection Failed\n");
        return -1;
    }// end if connect
    printf("%.24s CLIENT: connected to %s:%i\n", ctime(&ticks), IP_ADDRESS, PORT);

    // CONNECTION ESTABLISHED W. SERVER!!!!!

    clientMsg = "Hello from client";
    send(socketFd , clientMsg , strlen(clientMsg) , 0 );
    printf("%.24s CLIENT: Hello message sent\n", ctime(&ticks));

    recv( socketFd , buf, 1024, 0);
    printf("%.24s CLIENT: %s\n", ctime(&ticks),buf );

    sleep(2); // Wait before closing the file descriptor.
    close(socketFd);

    return 0;
}// end method main
