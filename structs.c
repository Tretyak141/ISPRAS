#include <stdlib.h>
#include <stdio.h>
#include "graphs.h"


void creating_pools(int counter_of_pools)
{
    pools = malloc(sizeof(pool)*counter_of_pools);
    for (int i=0;i<counter_of_pools;i++)
    {
        pools[i] = create_pool();
    }
}

char create_conn(int num1,int num2)
{
    if (is_connected(num1,num2)) return 1;

    add_edge(num1,num2);
    add_edge(num2,num1);
    return 0;
}

char delete_conn(int num1,int num2) 
{
    if (!is_connected(num1,num2)) return 1;
    delete_edge(num1,num2);
    delete_edge(num2,num1);
    return 0;
}

void add_water(int num1,int water)
{
    pools[num1].volume += water;
}

double show_water(int num)
{
    return pools[num].volume;
}

void distributor_for_all_components(int num_of_pools)
{
    for (int i=0;i<num_of_pools;i++)
    {
        if (pools[i].end_edges == pools[i].start_edges) continue; //if it's point with no edges
        if (!pools[i].distributed)
        {
            double average_volume = group_water(i);
            redist_water(i,average_volume);
        }
    }
    for (int i=0;i<num_of_pools;i++)
    {
        pools[i].distributed = 0;
    }
}

void free_everything(int numb)
{
    for (int i=0;i<numb;i++)
    {
        free(pools[i].edges);
    }
    free(pools);
}