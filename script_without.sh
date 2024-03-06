#!/bin/bash

gcc  -c stack.c -o stack.o && \

gcc -c incapsulated.c -o incapsulated.o && \

gcc -c structs.c -o structs.o &&\

gcc main_without_printing.c incapsulated.c structs.c stack.c -o result_without && \

./result_without 