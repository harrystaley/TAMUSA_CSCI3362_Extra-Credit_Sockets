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
#include <errno.h>

int main(int argc, char const *argv[])
{
    char *ip_address_c; // Defines the IP Address for the local host
    char  *port_c; // Defines the port that will be used.
    char * char_sock_addr; // Defines the socket address that the user defined in teh argument.
    int socketFd = 0; // socket file descriptor
    struct sockaddr_in serverAddr;
    char *clientMsg;
    char buf[1024] = {0};
    time_t ticks = time(NULL);

    // checks to see if the user provided an argument to parse.
    if (argc < 1) {
        fprintf(stderr,"CLIENT ERROR, Please provide your address in the following format: command ip_address:port\n ex: client 127.0.0.1:80\n");
        exit(1);
    } else {
       char_sock_addr = (char *) argv[1];
        const char delin[2] = ":";
        char* token;
        token = strtok(char_sock_addr, delin);// parse ip address
        ip_address_c = token;
        token = strtok(NULL, delin);// parse port number.
        port_c = token;
    }// end if argc != 1

    sleep(1); // Wait for one second before trying to connect to the server.

    /*
     * CREATE SOCKET FILE DESCRIPTOR
     *
     * Create a socket file descriptor as defined by:
     * AF_INET = IPv4
     * SOCK_STREAM = TCP
     * 0 = Defines IP Protocol as it appears on protocol field in the IP header of a packet.
     */

    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("CLIENT ERROR: Socket creation error on client\n");
        return -1;
    }// end if socket


    printf("CLIENT: socket created\n");

    memset(&serverAddr, '0', sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    // Convert IPv4 or IPv6 addresses from text to binary
    if(inet_pton(AF_INET, ip_address_c, &serverAddr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }// end if inet_ption

    /* Convert the original port from string to long and check for errorrs in port range errors. */
    char *eptr;
    long port;
    port = strtol(port_c, &eptr, 10);
    /* If the result is 0, test for an error or if port is out of the range of acceptable ports. */
    if (port == 0 || port < 0 || port > 65535 ) {
        /* If a conversion error occurred, display a message and exit */
        if (errno == EINVAL) {
            printf("CLIENT ERROR: Conversion error occurred: %d\n", errno);
            exit(0);
        }// if errno == EINVAL

        /* If the value provided was out of range, display a warning message */
        if (errno == ERANGE) {
            printf("CLIENT ERROR: The value provided was out of range\n");
        } // end if errorno == ERANGE
        // check to see if the port is out of the range of acceptable ports.
        if (port < 0 || port > 65535) {
            printf("CLIENT ERROR: The port number provided was out of range and must be between 0 and 65535\n");
        }// if port < 0 || port > 65535
    }
    serverAddr.sin_port = htons(port);

    //printf("CLIENT: converted %s to binary.\n", IP_ADDRESS);

    /*
    * CONNECT TO THE SERVER
     *
     * The connect() system call connects the socket referred to by the file descriptor socketFd to the address
     * specified by serverAddr where the server’s address and port is specified in serverAddr.
     */

    if (connect(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("CLIENT ERROR: Connection Failed\n");
        return -1;
    }// end if connect
    printf("%.24s CLIENT: connected to %s:%s\n", ctime(&ticks), ip_address_c, port_c);

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
