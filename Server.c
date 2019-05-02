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
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char const *argv[]) {
    struct sockaddr_in sockAddrIn;
    int socketFd; // server socket file descriptor
    int queLen = 3; // the number of connections slots in the listen que.
    int connSocketFd; // new socket file descriptor
    char * char_sock_addr; // Defines the socket address that the user defined in teh argument.
    char *ip_address_c; // Defines the IP Address for the local host
    char *port_c; // Defines the port that will be used.
    int optEnabled = 1;
    int addressLen = sizeof(sockAddrIn);
    char recvBuf[1024] = {0};
    char sendBuf[1024] = {0};
    time_t ticks = time(NULL);

    if (argc < 1) {
        fprintf(stderr,"SERVER ERROR, Please provide your address in the following format: command ip_address:port\n ex: server 127.0.0.1:80\n");
        exit(1);
    } else {
        char_sock_addr = (char *) argv[1];
        const char delin[2] = ":";
        ip_address_c = strtok(char_sock_addr, delin);// parse ip address
        port_c = strtok(NULL, delin);// parse port number
    }// end if argc != 1

    /*
     * CREATE SOCKET FILE DESCRIPTOR
     *
     * Create a socket file descriptor as defined by:
     * AF_INET = IPv4
     * SOCK_STREAM = TCP
     * 0 = Defines IP Protocol as it appears on protocol field in the IP header of a packet.
     */

    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("SERVER ERROR: socket creation failed\n");
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

    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optEnabled, sizeof(optEnabled))) {
        perror("SERVER ERROR: setsockopt failed\n");
        exit(EXIT_FAILURE);
    }// end if setsockopt

    sockAddrIn.sin_family = AF_INET;

    // Convert IPv4 or IPv6 addresses from text to binary
    if(inet_pton(AF_INET, ip_address_c, &sockAddrIn.sin_addr)<=0) {
        printf("SERVER ERROR: Invalid address/ Address not supported\n");
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
            printf("SERVER ERROR: Conversion error occurred: %d\n", errno);
            exit(0);
        }// if errno == EINVAL

        /* If the value provided was out of range, display a warning message */
        if (errno == ERANGE) {
            printf("SERVER ERROR: The value provided was out of range\n");
            exit(0);
        } // end if errorno == ERANGE
        // check to see if the port is out of the range of acceptable ports.
        if (port < 0 || port > 65535) {
            printf("SERVER ERROR: The port number provided was out of range and must be between 0 and 65535\n");
            exit(0);
        }// if port < 0 || port > 65535
    }
    sockAddrIn.sin_port = htons(port);

    //printf("SERVER: Socket Options Set\n");

    /*
     * BIND THE SOCKET
     *
     * The bind() function binds the socket to the address and port number specified in sockAddrIn. in this project, the
     * server is bound to localhost which is why we use INADDR_ANY to refer to the IP address.
     */

    if (bind(socketFd, (struct sockaddr *)&sockAddrIn, sizeof(sockAddrIn))<0) {
        perror("SERVER ERROR: bind failed\n");
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

    if (listen(socketFd, queLen) < 0) {
        perror("SERVER ERROR: listen failed\n");
        exit(EXIT_FAILURE);
    }// end if listen
    printf("\n%.24s SERVER MESG: Listening on %s:%li\n", ctime(&ticks),ip_address_c, port);


    /*
     * ACCEPT THE CONNECTION
     *
     * The accept() system call extracts the first connection request on the queue of pending connections for the listening socket and socket file descriptor.
     * Then creates a new connected socket, and returns a new file descriptor referring to that socket.
    */

     if ((connSocketFd = accept(socketFd, (struct sockaddr *)&sockAddrIn, (socklen_t*)&addressLen))<0) {
        perror("SERVER ERROR: accept.\n");
        exit(EXIT_FAILURE);
    } else {
         printf("%.24s SERVER MESG: Connection accepted.\n", ctime(&ticks));
     }

    // CONNECTION ESTABLISHED W. CLIENT!!!!!
    // Keep listening and transmitting while the client is connected.
    while(1) {
        // Receve data from the client.
         ssize_t r = recv(connSocketFd, recvBuf, sizeof(recvBuf), 0);
        if (r == 0|| strcmp(recvBuf, "EXIT\n")==0){
            printf("%.24s SERVER MESG: Client disconnected.\n", ctime(&ticks));
            break;
        } else if (r == -1) {
            printf("SERVER ERROR: Recv Error.\n");
        } else {
            printf("%.24s SERVER RECV: %s", ctime(&ticks), recvBuf);
            memset(recvBuf,0,sizeof(recvBuf));
        }// end if r == 0

        // send data to the client.
        printf(">>>>>>>>>>>: ");
        fgets(sendBuf, sizeof(sendBuf), stdin);
        ssize_t s = send(connSocketFd, sendBuf, strlen(sendBuf), 0);
        if (s == -1){
            printf("SERVER ERROR: Send Error.\n");
        } else {
            printf("%.24s SERVER SENT: %s", ctime(&ticks), sendBuf);
            memset(sendBuf,0,sizeof(sendBuf));
        }

    }// end while true.
    close(socketFd); // close the socket.
    close(connSocketFd); // Close the connection socket.

    return 0;
}// end method main
