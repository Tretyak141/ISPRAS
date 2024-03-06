#ifndef _structs_h_
#define _structs_h_

#include <stdlib.h>
#include <stdio.h>

/**
 * @return Function create_pool return pool with settings 0 in all fields
*/

void creating_pools(int counter_of_pools);


/**
 * @param num Num is a number of pool in array of pools
 * @return Returnable parametr is a volume of num's pool
*/

double show_water(int num);

/**
 * Create connection between num1 and num2 parametrs
 * Create new edge
 * Return 0 if edge was created
 * Return 1 if edge was created earlier
*/

int create_conn(int num1,int num2);

/**
 * Delete connection between num1 and num2 parametrs
 * Delete edge
 * Return 0 if edge was deleted
 * Return 1 if there's no edge between num1 and num2
*/

int delete_conn(int num1,int num2);

/**
 * Adding water to num1 pool
*/

void add_water(int num1, int water);

/**
 * Distribution for all the components after adding water
*/

void distributor_for_all_components(int num_of_pools);

/**Delete all allocated memory
 * Delete information about edge ans also
 * about pools
*/

void free_everything(int numb);
#endif