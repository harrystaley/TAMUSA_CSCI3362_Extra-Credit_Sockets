#!/bin/sh

# Run the C files.
./server & 192.168.1.101:5000
./client & 192.168.1.101:5000
