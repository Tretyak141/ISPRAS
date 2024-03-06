#!/bin/bash

gcc -c stack.c -o stack.o

gcc -c incapsulated.c -o incapsulated.o

gcc  -c structs.c -o structs.o && \

gcc -c main.c -o main.o

gcc main.c incapsulated.c structs.c stack.c -o result && \

./result