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



    printf("First step\n\n");

    printf("Start creating pools\n");
    for (int i=0;i<N;i++)
    {
        pools[i] = create_pool();
        //printf("Pool #%d created\n",i);
    }
    
    printf("\nSecond step\n\n");
    
    printf("Adding water\n");
    for (int i=0;i<N;i++)
    {
        double volume = 1 + rand()%500;
        volume /= counting(i);
        set_nulls(i);
        add_water(i,volume);
        set_nulls(i);
        //printf("%lf litres was added to pool %d\n",volume, i);
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
        printf("%d Chanel %d-%d was created\n",i,num1,num2);
    }



    printf("\nFourth step\n\n");
    printf("Measure water\n");
    for (int i=0;i<N;i++)
    {
        //printf("Volume of %d pool is %lf\n",i,pools[i].volume);
    }

    printf("\nFifth step\n\n");
    printf("Adding water\n");
    for (int i=0;i<L;i++)
    {
        int num = rand()%N;
        double water = WMIN + rand()%WMAX;
        //printf("%lf liters was added to pool #%d\n",water,num);
        add_water(num,water);
    }

    for (int i=0;i<N;i++)
    {
        if (!pools[i].distributed)
        {
            double volume = check_water(i);
            set_nulls(i);
            volume /= counting(i);
            set_nulls(i);
            redist_water(i,volume);
            set_nulls(i);
        }
    }

    for (int i=0;i<N;i++)
        pools[i].distributed = 0;

    printf("\nSixth step\n\n");
    printf("Measuring againg\n");

    for (int i=0;i<N;i++)
    {
        //printf("Volume of %d pool is %lf\n",i,pools[i].volume);
    }

    printf("\nSeventh step\n\n");
    printf("Destroy chanels\n");

    for (int i=0;i<M;i++)
    {
        int flag = 1;
        int num1;
        int num2;

        while (flag)
        {
            num1 = rand()%N;
            num2 = rand()%N;
            if (num1!=num2)
                flag = delete_conn(num1,num2);
        }
    
        printf("%d Chanel %d-%d was deleted\n",i,num1,num2);
    }

    printf("\nEigth step\n\n");
    printf("Adding water\n");

    for (int i=0;i<L;i++)
    {
        int num = rand()%N;
        double water = WMIN + rand()%WMAX;
        //printf("%lf liters was added to pool #%d\n",water,num);
        add_water(num,water);
    }

    for (int i=0;i<N;i++)
    {
        if (!pools[i].distributed)
        {
            double volume = check_water(i);
            set_nulls(i);
            volume /= counting(i);
            set_nulls(i);
            redist_water(i,volume);
            set_nulls(i);
        }
    }
    for (int i=0;i<N;i++)
        pools[i].distributed = 0;


    printf("\nNinth step\n\n");
    printf("Measuring again\n");

    for (int i=0;i<N;i++)
    {
        //printf("Volume of %d pool is %lf\n",i,pools[i].volume);
    }    
    free_everything(N);
    time_t end = clock() - start;
    printf("\n\nTime: %lf\n\n",(double)end/CLOCKS_PER_SEC);
}

