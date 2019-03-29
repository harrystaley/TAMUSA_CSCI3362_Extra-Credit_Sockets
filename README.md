#Simple Example of the use of Sockets

## Requirements

- Install Virtual Box
- Install Vagrant

##How to Run This Project

1. Once Vagrant and Virtual Box are installed clone this repo.'
2. Once cloned navigate to that folder and run the vagrantfile by:
'vagrant up'
3. Once complete ssh into your new server via the following command.
'vagrant ssh'
4. Navigate to the directory shared by your host OS located at /vagrant.
'cd /vagrant'
5. Install the Gnu C Compiler.
'sudo apt-get upate'
'sudo apt-get install gcc'
6. Run the provided shell script that will compile and run the client and server programs.
'bash ./Run_Socket_Test.sh'



