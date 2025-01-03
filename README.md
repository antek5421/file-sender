# file-sender

## Overview

This project demonstrates a simple file transfer system using socket programming. It consists of two main components:

- **Server**: Receives and saves files sent by the client.
- **Client**: Sends files to the server over a network connection.

The client-server communication is based on TCP sockets, and the project is implemented in C.

## Features

- **File transfer**: Allows sending and receiving files over a TCP connection.
- **Socket communication**: Utilizes low-level socket programming to establish a connection between client and server.
- **Cross-platform**: This project should work on any UNIX-like system (Linux, macOS) with a C compiler.
- **Error handling**: Basic error checks are implemented for file operations and socket communications.
- **AES Encryption**: Encrypts files during transfer to ensure data security.


## Planned Features

In future updates, the following features are planned to enhance the functionality of the file transfer system:

- **Multithreading**: Improve performance by enabling the server to handle multiple clients simultaneously using threads.

## Requirements

- A C compiler (e.g., GCC)
- A terminal/command line environment
- Basic understanding of socket programming in C
- OpenSSL Library (for handling hashing functions and security)
- POSIX-Compliant Operating System (Linux, macOS, or Windows with WSL)
  
## Installation

### Clone the repository

```bash
git clone https://github.com/your-username/file-transfer.git
cd file-transfer
```

## Usage

### Running the Server

To start the server, run the following command:

```bash
./server -s <filename>
```
Where <filename> is the name of the file the server will save once received from the client.

### Running the Client

To send a file to the server, use the following command:

```bash
./client -c <server-ip> <filename>
```
Where:
    - <server-ip> is the IP address of the server.
    - <filename> is the path to the file you want to send. 
