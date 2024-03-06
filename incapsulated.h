#ifndef _incapsulated_h_
#define _incapsulated_h_
#define N 25000000

#include "stack.h"

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

pool *pools;

pool create_pool();


void set_nulls(int num1);

/**
 * Function for counting points in connectivity component
 * Need in redistribution of water
*/

int counting(int num1);

/**
 * Counting average volume in connectivity component
*/

double group_water(int num1);


/**
 * Redistribution water in one connectivity component
 * @param volume New volume of group. Counting as group_water
*/

void redist_water(int num1, double volume);

#endif