#!/bin/bash

gcc  -c structs.c -o structs.o && \

gcc main.c structs.c -o result && \

valgrind ./result