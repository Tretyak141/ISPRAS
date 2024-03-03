#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

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

/**
 * Algorithms of counting volume,
 * connected pools and also algorithm
 * of redistribution water based on classic
 * algorithm DFS
 * It isn't needed extra_memory and it helps
 * count every needed nums effective
*/

double show_water(int num)
{
    return pools[num].volume;
}

int counting(int num1)
{
    if (pools[num1].passed) return 0;
    int ans = 1;
    pools[num1].passed = 1;
    for (int i=pools[num1].start_edges;i<pools[num1].end_edges;i++)
    {
        if (pools[num1].edges[i]!=-1)
        {
            if (!pools[pools[num1].edges[i]].passed)
            {
                ans += counting(pools[num1].edges[i]);
            }
        }
    }
    return ans;
}

int redist_water(int num1, double volume)
{
    if (pools[num1].passed) 1;
    pools[num1].passed = 1;
    pools[num1].distributed = 1;
    pools[num1].volume = volume;
    for (int i=pools[num1].start_edges;i<pools[num1].end_edges;i++)
    {
        if (pools[num1].edges[i]!=-1)
        {
            if (!pools[pools[num1].edges[i]].passed)
            {
                redist_water(pools[num1].edges[i],volume);
            }
        }
    }
}

/**
 * Setting 0 after DFS
 * Using... DFS)))
*/

void set_nulls(int num1)
{
    if (!pools[num1].passed) return;
    pools[num1].passed = 0;

    for (int i=pools[num1].start_edges;i<pools[num1].end_edges;i++)
    {
        if (pools[num1].edges[i]!=-1)
        {
            if (pools[pools[num1].edges[i]].passed)
            {
                set_nulls(pools[num1].edges[i]);
            }
        }
    }
}

double check_water(int num1)
{
    if (pools[num1].passed) return 0;
    double ans = pools[num1].volume;
    pools[num1].passed = 1;

    for (int i=pools[num1].start_edges;i<pools[num1].end_edges;i++)
    {
        if (pools[num1].edges[i]!=-1)
        {
            if (!pools[pools[num1].edges[i]].passed)
            {
                ans += check_water(pools[num1].edges[i]);
            }
        }
    }
    return ans;
}

int create_conn(int num1,int num2)
{
    qsort(pools[num1].edges,pools[num1].end_edges,sizeof(int),cmp);
    if (bsearch(&num2,pools[num1].edges,pools[num1].end_edges,sizeof(int),cmp)) return 1;
    pools[num1].end_edges ++;
    if (!pools[num1].edges)
    {
        pools[num1].edges = malloc(sizeof(int));
    }
    else
    {
        pools[num1].edges = realloc(pools[num1].edges,sizeof(int)*pools[num1].end_edges);
    }
    pools[num1].edges[pools[num1].end_edges - 1] = num2;

    pools[num2].end_edges ++;
    if (!pools[num2].edges)
    {
        pools[num2].edges = malloc(sizeof(int));
    }
    else
    {
        pools[num2].edges = realloc(pools[num2].edges,sizeof(int)*pools[num2].end_edges);
    }
    pools[num2].edges[pools[num2].end_edges - 1] = num1;

    qsort(pools[num1].edges,pools[num1].end_edges,sizeof(int),cmp);
    qsort(pools[num2].edges,pools[num2].end_edges,sizeof(int),cmp);
    return 0;
}

int delete_conn(int num1,int num2)
{
    qsort(pools[num1].edges,pools[num1].end_edges,sizeof(int),cmp);
    int *f1;
    if (!(f1 = bsearch(&num2,pools[num1].edges,pools[num1].end_edges,sizeof(int),cmp))) return 1;
    *f1 = -1;
    pools[num1].start_edges ++;

    qsort(pools[num2].edges,pools[num2].end_edges,sizeof(int),cmp);

    int *f2;
    f2 = bsearch(&num1,pools[num2].edges,pools[num2].end_edges,sizeof(int),cmp);
    *f2 = -1;
    pools[num2].start_edges++;
    qsort(pools[num1].edges,pools[num1].end_edges,sizeof(int),cmp);
    qsort(pools[num2].edges,pools[num2].end_edges,sizeof(int),cmp);


    return 0;
}



void add_water(int num1,double water)
{
    pools[num1].volume += water;
}

void free_everything(int numb)
{
    for (int i=0;i<numb;i++)
    {
        free(pools[i].edges);
    }
    free(pools);
}