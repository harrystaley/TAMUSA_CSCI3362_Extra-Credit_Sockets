#!/bin/sh

# Run the C files.
./server 127.0.0.1:500000 &
./client 127.0.0.1:500000 &
