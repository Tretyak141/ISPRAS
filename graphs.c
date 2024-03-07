#include "stack.h"
#include <stdlib.h>

struct pool
{
    double volume;
    int *edges;
    int start_edges;
    int end_edges;
    char passed;
    char distributed;
};

typedef struct pool pool;

extern pool *pools;

int cmp(const void *num1,const void *num2)
{
    if ((*(int*)num1)>(*(int*)num2)) return 1;
    if ((*(int*)num1)<(*(int*)num2)) return -1;
    return 0;
}

pool create_pool()
{
    pool ret;
    ret.volume = 0;
    ret.distributed = 0;
    ret.edges = NULL;
    ret.passed = 0;
    ret.start_edges = 0;
    ret.end_edges = 0;
    return ret;
}

char is_connected (int num1,int num2)
{
    return (bsearch(&num2,pools[num1].edges,pools[num1].end_edges,sizeof(int),cmp) != NULL)  //num1-num2 edge
        || 
            (bsearch(&num1,pools[num2].edges,pools[num2].end_edges,sizeof(int),cmp) != NULL);  //num2-num1 edge
}

void add_edge(int num1, int num2)
{
    int count_connected = pools[num1].end_edges;
    count_connected ++;
    
    if (count_connected==1)
        pools[num1].edges = malloc(sizeof(int));
    else
        pools[num1].edges = realloc(pools[num1].edges,sizeof(int)*count_connected);
    pools[num1].end_edges++;
    pools[num1].edges[count_connected-1] = num2;
    qsort(pools[num1].edges,count_connected,sizeof(int),cmp);
}

void delete_edge(int num1,int num2)
{
    int counter_connected = pools[num1].end_edges;
    int *founded = bsearch(&num2,pools[num1].edges,counter_connected,sizeof(int),cmp);
    *founded = -1;
    qsort(pools[num1].edges,counter_connected,sizeof(int),cmp);
    pools[num1].start_edges++;
}

void set_nulls(int num1)
{
    push_stack(num1);
    pools[num1].passed = 0;
    while (stack_size())
    {
        int current_num = pop_stack();
        int start_position = pools[current_num].start_edges;
        int end_position = pools[current_num].end_edges;
        for (int i=start_position;i<end_position;i++)
        {
            int num_of_point = pools[current_num].edges[i];
            if (pools[num_of_point].passed)
            {
                pools[num_of_point].passed = 0;
                push_stack(num_of_point);
            }
        }
    }
}

int counting(int num1)
{
    int ans = 1;
    push_stack(num1);
    pools[num1].passed = 1;
    while (stack_size())
    {
        int current_num = pop_stack();

        int start_position = pools[current_num].start_edges;
        int end_position = pools[current_num].end_edges;
        for (int i=start_position;i<end_position;i++)
        {
            int num_of_point = pools[current_num].edges[i];
            if (!pools[num_of_point].passed)
            {
                ans++;
                pools[num_of_point].passed = 1;
                push_stack(num_of_point);
            }
        }
    }
    set_nulls(num1);
    return ans;
}


double group_water(int num1)
{
    double ans = pools[num1].volume;
    push_stack(num1);
    pools[num1].passed = 1;
    while (stack_size())
    {
        int current_num = pop_stack();
        int start_position = pools[current_num].start_edges;
        int end_position = pools[current_num].end_edges;
        for (int i=start_position;i<end_position;i++)
        {
            int num_of_point = pools[current_num].edges[i];
            if (!pools[num_of_point].passed)
            {
                ans += pools[num_of_point].volume;
                pools[num_of_point].passed = 1;
                push_stack(num_of_point);
            }
        }
    }
    set_nulls(num1);
    ans /= counting(num1);
    return ans;
}

void redist_water(int num1, double volume)
{
    push_stack(num1);
    pools[num1].volume = volume;
    pools[num1].passed = 1;
    pools[num1].distributed = 1;
    while (stack_size())
    {
        int current_num = pop_stack();
        int start_position = pools[current_num].start_edges;
        int end_position = pools[current_num].end_edges;
        for (int i=start_position;i<end_position;i++)
        {
            int num_of_point = pools[current_num].edges[i];
            if (!pools[num_of_point].passed)
            {
                pools[num_of_point].passed = 1;
                pools[num_of_point].distributed = 1;
                pools[num_of_point].volume = volume;
                push_stack(num_of_point);
            }
        }
    }
    set_nulls(num1);
}

