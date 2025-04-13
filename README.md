# TAMUSA_CSCI3362_Extra-Credit_Sockets

## Project Overview
This repository is designed for educational purposes to demonstrate the use of sockets within a virtualized environment. It includes simple server and client applications that communicate over sockets, showcasing basic network interactions. The project utilizes Vagrant and VirtualBox to create a consistent and controlled virtual environment where these applications can be run and tested.

### Project Structure
```
TAMUSA_CSCI3362_Extra-Credit_Sockets/
│
├── Vagrantfile           # Configuration file for Vagrant to setup the virtual environment
├── server                # Directory containing the server application
│   └── server.c          # Server application source code
│
├── client                # Directory containing the client application
│   └── client.c          # Client application source code
│
└── README.md             # Documentation for the project
```

## Setup and Installation

### Dependencies
- **VirtualBox**: This project uses VirtualBox as the provider for the virtual machines. Download and install it from [VirtualBox.org](https://www.virtualbox.org/).
- **Vagrant**: Vagrant is used to manage the virtual environment. Install it from [Vagrantup.com](https://www.vagrantup.com/downloads).

### Installation Steps
1. **Clone the Repository**
   ```bash
   git clone https://github.com/your-username/TAMUSA_CSCI3362_Extra-Credit_Sockets.git
   cd TAMUSA_CSCI3362_Extra-Credit_Sockets
   ```

2. **Start the Virtual Environment**
   ```bash
   vagrant up
   ```

3. **SSH into the Virtual Machine**
   ```bash
   vagrant ssh
   ```

4. **Compile the Applications**
   Navigate to the synced folder in the VM, then compile the server and client applications:
   ```bash
   cd /vagrant/server
   gcc server.c -o server
   cd ../client
   gcc client.c -o client
   ```

## Usage Examples

### Running the Server
From the server directory in the virtual machine:
```bash
./server
```

### Running the Client
From the client directory in the virtual machine (ensure the server is running first):
```bash
./client
```

## Contributing

Contributions to this project are welcome! Here's how you can contribute:
1. **Fork the Repository**: Click the 'Fork' button at the top right of this page.
2. **Clone Your Fork**: `git clone https://github.com/your-username/TAMUSA_CSCI3362_Extra-Credit_Sockets.git`
3. **Create a New Branch**: `git checkout -b your-branch-name`
4. **Make Your Changes**: Enhance the code or documentation and commit your changes.
5. **Push Your Changes**: `git push origin your-branch-name`
6. **Submit a Pull Request**: Open a pull request from your forked repository to the original repository.

Please ensure your code adheres to the existing style of the project to maintain consistency.

## License

This project is available under the MIT License. See the LICENSE file for more details.