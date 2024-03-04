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
    pools = malloc(N*sizeof(pool));
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

    int counter = 0;

    printf("First step\n\n");

    printf("Start creating pools\n");
    for (int i=0;i<N;i++)
    {
        pools[i] = create_pool();
        printf("Pool #%d created\n",i);
    }
    
    printf("\nSecond step\n\n");
    
    printf("Adding water\n");
    for (int i=0;i<N;i++)
    {
        int volume = 1 + rand()%500;
        add_water(i,volume);
        printf("%d litres was added to pool %d\n",volume, i);
    }

    printf("\nThird step\n\n");
    printf("Creating chanels\n");

    for (int i=0;i<K;i++)
    {
        int flag = 1;
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
        printf("%d Channel %d-%d was created\n",i,num1,num2);

        //Creating information about channels
        //conns[i][0] - first param
        //conns[i][1] - second param
        conns[counter][0] = num1;
        conns[counter][1] = num2;
        counter++;
    }

    distributor_for_all_components(N);

    printf("\nFourth step\n\n");
    printf("Measure water\n");
    for (int i=0;i<N;i++)
    {
        printf("Volume of %d pool is %lf\n",i,pools[i].volume);
    }

    printf("\nFifth step\n\n");
    printf("Adding water\n");
    for (int i=0;i<L;i++)
    {
        int num = rand()%N;
        int water = WMIN + rand()%WMAX;
        printf("%d liters was added to pool #%d\n",water,num);
        add_water(num,water);
    }

    distributor_for_all_components(N);

    printf("\nSixth step\n\n");
    printf("Measuring againg\n");

    for (int i=0;i<N;i++)
    {
        printf("Volume of %d pool is %lf\n",i,pools[i].volume);
    }

    printf("\nSeventh step\n\n");
    printf("Destroy chanels\n");

    for (int i=0;i<M;i++)
    {

        int num_con;
        int flag = 1;
        while(flag)
        {
            num_con = rand()%counter;
            if (conns[num_con][0]!=-1)
            flag = delete_conn(conns[num_con][0],conns[num_con][1]);
        }
        conns[num_con][0] = -1;
        printf("%d Chanel %d-%d was deleted\n",i,conns[num_con][0],conns[num_con][1]);
    }

    printf("\nEigth step\n\n");
    printf("Adding water\n");

    for (int i=0;i<L;i++)
    {
        int num = rand()%N;
        int water = WMIN + rand()%WMAX;
        printf("%d liters was added to pool #%d\n",water,num);
        add_water(num,water);
    }

    printf("\nNinth step\n\n");
    printf("Measuring again\n");

    for (int i=0;i<N;i++)
    {
        printf("Volume of %d pool is %lf\n",i,pools[i].volume);
    }    
    free_everything(N);

    for (int i=0;i<K;i++)
    {
        free(conns[i]);
    }
    free(conns);

    time_t end = clock() - start;
    printf("\n\nTime: %lf\n\n",(double)end/CLOCKS_PER_SEC);
}

