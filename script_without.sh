#!/bin/bash

gcc  -c structs.c -o structs.o && \

gcc main_without_printing.c structs.c -o result_without && \

./result_without