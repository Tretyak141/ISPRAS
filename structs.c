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

pool *pools;

pool create_pool()
{
    pool ret;
    ret.volume = 0;
    ret.edges = NULL;
    ret.start_edges = 0;
    ret.end_edges = 0;  
    ret.passed = 0;    
    ret.distributed = 0;
    return ret;
}


int cmp(const void *num1,const void *num2)
{
    if ((*(int*)num1)>(*(int*)num2)) return 1;
    if ((*(int*)num1)<(*(int*)num2)) return -1;
    return 0;
}

void set_nulls(int num1)
{
    if (!pools[num1].passed) return;
    pools[num1].passed = 0;

    for (int i=pools[num1].start_edges;i<pools[num1].end_edges;i++)
    {
        int num_of_neigh = pools[num1].edges[i];

        if (pools[num_of_neigh].passed)
        {
            set_nulls(num_of_neigh);
        }
    }
}

/**
 * Function for counting points in connectivity component
 * Need in redistribution of water
*/

int counting(int num1, int ind)
{
    if (pools[num1].passed) return 0;
    int ans = 1;
    pools[num1].passed = 1;
    for (int i=pools[num1].start_edges;i<pools[num1].end_edges;i++)
    {
        int num_of_neigh = pools[num1].edges[i]; 
        if (!pools[num_of_neigh].passed)
        {
            ans += counting(num_of_neigh,1);
        }
    }
    if (!ind) set_nulls(num1);
    return ans;
}

/**
 * Redistribution water in one connectivity component
 * @param ind Indicator of start point (0 - start point)
 * @param volume New volume of group. Counting as group_water
*/

void redist_water(int num1, double volume, int ind)
{
    if (pools[num1].passed) 1;
    pools[num1].passed = 1;
    pools[num1].distributed = 1;

    pools[num1].volume = volume;
    
    for (int i=pools[num1].start_edges;i<pools[num1].end_edges;i++)
    {
        int num_of_neigh = pools[num1].edges[i];
        if (!pools[num_of_neigh].passed)
        {
            redist_water(num_of_neigh,volume,1);
        }
    }
    if (!ind)
        set_nulls(num1);
}


/**
 * Counting average volume in connectivity component
 * @param ind Indicator of start point
*/

double group_water(int num1, int ind)
{
    if (pools[num1].passed) return 0;
    double ans = pools[num1].volume;
    pools[num1].passed = 1;

    for (int i=pools[num1].start_edges;i<pools[num1].end_edges;i++)
    {

        int num_of_neigh = pools[num1].edges[i];

        if (!pools[num_of_neigh].passed)
        {
            ans += group_water(num_of_neigh,1);
        }
    }
    if (!ind)
    {
        set_nulls(num1);
        ans /= counting(num1,0);
    }

    return ans;
}



/**
 * All function under this commentary was declared in structs.h
*/



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
    qsort(pools[num1].edges, pools[num1].end_edges, sizeof(int),cmp);
    int *f1;

    int counter_connected1 = pools[num1].end_edges;
    int counter_connected2 = pools[num2].end_edges;

    if (!(f1 = bsearch(&num2,pools[num1].edges, counter_connected1, sizeof(int),cmp))) return 1;
    
    *f1 = -1;
    
    pools[num1].start_edges ++;

    qsort(pools[num2].edges, counter_connected2, sizeof(int), cmp);

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
        if (!pools[i].distributed)
        {
            double average_volume = group_water(i,0);
            redist_water(i,average_volume,0);
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