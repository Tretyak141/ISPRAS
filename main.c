#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "structs.h"

#define N 25000000
#define K 10000000
#define L 30000000
#define M 2000000
#define WMIN 1
#define WMAX 500

int main()
{
    time_t start = clock();
    srand(time(NULL));

    /**
     * conns has an information about created channels
     * counter - num of created channels
     * conns[i][0] - first elem of channel
     * conns[i][1] - second elem of channel
    */
    int **conns;
    
    conns = malloc(sizeof(int*)*K);
    for (int i=0;i<K;i++)
        conns[i] = malloc(sizeof(int)*2);

    
    creating_pools(N);


    for (int i=0;i<N;i++)
    {
        int volume = WMIN + rand()%WMAX;
        add_water(i,volume);
    }


    for (int i=0;i<K;i++)
    {
        char flag = 1;
        int num1;
        int num2;
        while (flag)
        {
            num1 = rand()%N;
            num2 = rand()%N;
            if (num1!=num2)
            {
                flag = create_conn(num1,num2);
            }
        }

        //Creating information about channels
        //conns[i][0] - first param
        //conns[i][1] - second param
        conns[i][0] = num1;
        conns[i][1] = num2;
    }

    distributor_for_all_components(N);

    for (int i=0;i<N;i++)
    {
        show_water(i);
    }
    
    for (int i=0;i<L;i++)
    {
        int num = rand()%N;
        int water = WMIN + rand()%WMAX;
        add_water(num,water);
    }

    distributor_for_all_components(N);

    for (int i=0;i<N;i++)
    {
        show_water(i);
    }

    for (int i=0;i<M;i++)
    {
        int num_con;
        char flag = 1;
        while(flag)
        {
            num_con = rand()%K;
            if (conns[num_con][0]!=-1)
            flag = delete_conn(conns[num_con][0],conns[num_con][1]);
        }
        conns[num_con][0] = -1;
    }

    for (int i=0;i<L;i++)
    {
        int num = rand()%N;
        int water = WMIN + rand()%WMAX;
        add_water(num,water);
    }
    distributor_for_all_components(N);


    for (int i=0;i<N;i++)
    {
        show_water(i);
    } 

    for (int i=0;i<K;i++)
    {
        free(conns[i]);
    }
    free_everything(N);
    free(conns);
    time_t end = clock();
    end -= start;
    printf("\n\nTime: %lf\n\n",(double)end/CLOCKS_PER_SEC);
}

