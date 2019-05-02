# Simple Example of the use of Sockets

## Requirements

- Install Virtual Box
- Install Vagrant

## How to Run This Project

1. Once Vagrant and Virtual Box are installed clone this repo.'
2. Once cloned navigate to that folder and run the vagrantfile by:
'vagrant up'
3. Once complete ssh into your new server via the following command.
'vagrant ssh'
4. Navigate to the directory shared by your host OS located at /vagrant.
'cd /vagrant'
6. Compile the server.
 
    ```gcc Server.c -O Server```
7. Run the server specifying the ip address and the port of the server..
    
    ```./Server <ip address>:<port>```
8. On the machine that you want to connect to the server.
9. Compile the client.
    
    ```gcc Client.c -O Client```
10. Run the client specifying the ip address and the port of the server..
    
    ```./Client <ip address>:<port>```
    



