# TAMUSA_CSCI3362_Extra-Credit_Sockets

## Project Overview

This repository is designed to provide a simple, yet effective, example of using sockets in a virtualized environment. It is a part of an extra credit assignment for the CSCI3362 course at Texas A&M University-San Antonio. The project makes use of Vagrant and VirtualBox to set up the environment and includes instructions for compiling and running both server and client applications to demonstrate socket communication.

## Setup and Installation

Before getting started, you will need to have the following dependencies installed on your machine:

- [Vagrant](https://www.vagrantup.com/)
- [VirtualBox](https://www.virtualbox.org/)

Once these dependencies are installed, you can clone this repository to your local machine. Navigate to the directory where you want to clone the repository and run the following command:

```bash
git clone https://github.com/username/TAMUSA_CSCI3362_Extra-Credit_Sockets.git
```

Navigate to the cloned repository and start the Vagrant environment:

```bash
cd TAMUSA_CSCI3362_Extra-Credit_Sockets
vagrant up
```

This will set up a virtualized environment for you to work with.

## Usage

Once your Vagrant environment is up and running, you can compile and run the server and client applications. 

To compile the server application, navigate to the server directory and run the make command:

```bash
cd server
make
```

To run the server application:

```bash
./server
```

Similarly, to compile the client application, navigate to the client directory and run the make command:

```bash
cd client
make
```

To run the client application:

```bash
./client
```

## Contributing

We welcome contributions from anyone. Before contributing, please read through our [contributing guidelines](CONTRIBUTING.md).

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for more details.