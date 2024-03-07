#!/bin/bash

gcc  -c stack.c -o stack.o && \

gcc -c graphs.c -o graphs.o && \

gcc -c structs.c -o structs.o &&\

gcc main.c graphs.c structs.c stack.c -o result_without && \

./result_without 