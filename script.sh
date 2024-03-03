#!/bin/bash

gcc -m64 -c structs.c -o structs.o && \

gcc -m64 main.c structs.c -o result && \

valgrind ./result