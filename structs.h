#ifndef _structs_h_
#define _structs_h_

#include <stdlib.h>
#include <stdio.h>

struct pool
{
    double volume;
    int *edges;
    int start_edges;
    int end_edges;
    int passed;
    int distributed;
};

typedef struct pool pool;

extern pool *pools;

pool create_pool();

int cmp(const void *num1,const void *num2);

double show_water(int num);

int counting(int num1);

double check_water(int num1);

/**
 * is_connected
 * 
 * Check ways from num1 to num2
 * Using DFS algorithm
*/

int is_connected(int num1, int num2);

/**
 * Setting 0 after DFS
 * Using... DFS)))
*/

void set_nulls(int num1);

void redist_water(int num1, double volume);

int create_conn(int num1,int num2);

int delete_conn(int num1,int num2);


void add_water(int num1,double water);

void free_everything(int numb);
#endif