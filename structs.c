#include <stdlib.h>
#include <stdio.h>
#include "incapsulated.h"


void creating_pools(int counter_of_pools)
{
    pools = malloc(sizeof(pool)*counter_of_pools);
    for (int i=0;i<counter_of_pools;i++)
    {
        pools[i] = create_pool();
    }
}

int cmp(const void *num1,const void *num2)
{
    if ((*(int*)num1)>(*(int*)num2)) return 1;
    if ((*(int*)num1)<(*(int*)num2)) return -1;
    return 0;
}

int create_conn(int num1,int num2)
{
    int counter_connected1 = pools[num1].end_edges;
    int counter_connected2 = pools[num2].end_edges;

    qsort(pools[num1].edges,counter_connected1,sizeof(int),cmp);

    if (bsearch(&num2,pools[num1].edges,counter_connected1,sizeof(int),cmp)) return 1;
    pools[num1].end_edges ++;
    counter_connected1++;

    if (!pools[num1].edges)
    {
        pools[num1].edges = malloc(sizeof(int));
    }
    else
    {
        pools[num1].edges = realloc(pools[num1].edges,sizeof(int)*counter_connected1);
    }

    pools[num1].edges[counter_connected1 - 1] = num2;

    pools[num2].end_edges ++;
    counter_connected2++;
    if (!pools[num2].edges)
    {
        pools[num2].edges = malloc(sizeof(int));
    }
    else
    {
        pools[num2].edges = realloc(pools[num2].edges,sizeof(int)*(counter_connected2));
    }
    pools[num2].edges[counter_connected2 - 1] = num1;

    qsort(pools[num1].edges,counter_connected1,sizeof(int),cmp);
    qsort(pools[num2].edges, counter_connected2, sizeof(int),cmp);
    return 0;
}

int delete_conn(int num1,int num2) 
{
    int *f1;

    int counter_connected1 = pools[num1].end_edges;
    int counter_connected2 = pools[num2].end_edges;

    if (!(f1 = bsearch(&num2,pools[num1].edges, counter_connected1, sizeof(int),cmp))) return 1;
    
    *f1 = -1;
    
    pools[num1].start_edges ++;

    int *f2;
    f2 = bsearch(&num1,pools[num2].edges, counter_connected2, sizeof(int),cmp);
    *f2 = -1;

    pools[num2].start_edges++;

    qsort(pools[num1].edges,counter_connected1,sizeof(int),cmp);
    qsort(pools[num2].edges,counter_connected2,sizeof(int),cmp);


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