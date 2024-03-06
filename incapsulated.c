#include "stack.h"
#include <stdlib.h>
#define N 25000000

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

