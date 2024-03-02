#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "structs.h"

#define N 3
#define K 2
#define L 2
#define M 1
#define WMIN 1
#define WMAX 500

int cmp_pare(const void *num1,const void *num2)
{
    if (((int*)(*(int**)num1))[0]>((int*)(*(int**)num2))[0])
    {

        return 1;
    }
    if (((int*)(*(int**)num1))[0]<((int*)(*(int**)num2))[0])
    {
        return -1;
    }
    if (((int*)(*(int**)num1))[1]>((int*)(*(int**)num2))[1])
    {
        return 1;
    }
    if (((int*)(*(int**)num1))[1]<((int*)(*(int**)num2))[1])
    {
        return -1;
    }
    return 0;
}

int main()
{
    srand(time(NULL));
    int **chanels_info;
    chanels_info = malloc(sizeof(int*)*K);
    pools = malloc(N*sizeof(pool));
    for (int i=0;i<K;i++)
    {
        chanels_info[i] = malloc(sizeof(int)*2);
        chanels_info[i][0] = -1;
        chanels_info[i][1] = -1;
    }
    for (int i=0;i<K;i++)
    {
        chanels_info[i][0] = INT_MAX;
        chanels_info[i][1] = INT_MAX;
    }

    printf("First and second step, creating and adding\n");
    for(int i=0;i<N;i++)
    {
        pools[i] = create();
        add_water(i,rand()%(WMAX - 1)+WMIN);
    }

    printf("Third step: making relations\n");

    for(int i=0;i<K;i++)
    {
        int flag = 1;
        while (flag)
        {
            int num1 = rand()%N;
            int num2 = num1 + rand()%(N-num1);
            if (num1==num2) continue;
            int *a;
            a = malloc(2*sizeof(int));
            a[0] = num1;
            a[1] = num2;                

            qsort(chanels_info,i,sizeof(int*),cmp_pare);

            if (!bsearch(&a,chanels_info,i,sizeof(int*),cmp_pare))
            {

                chanels_info[i][0] = num1;
                chanels_info[i][1] = num2;
                printf("CREATED %d %d\n",num1,num2);
                create_channel(num1,num2);
                flag = 0;
            }
            free(a);
        }
    }

    printf("Fourth step: measuring\n");

    for (int i=0;i<N;i++)
    {
        printf("Volume of %d pool is %lf\n\n",i,measure_volume(i));
    }

    printf("Fifth step: adding water\n");

    for (int i=0;i<L;i++)
    {
        double x;
        int num;
        add_water(num = rand()%N,x=rand()%(WMAX - 1)+WMIN);
        printf("ADDED %lf to %d\n",x,num);
    }

    printf("Sixth step: measuring\n");
    for (int i=0;i<N;i++)
    {
        printf("%d\n",pools[i].num_of_gr);
        printf("Volume of %d pool is %lf\n\n",i,measure_volume(i));
    }

    printf("Seventh step: destroy chanels\n");

    for (int i=0;i<M;i++)
    {
        int flag = 1;
        while (flag)
        {
            int num1 = rand()%N;
            int num2 = num1 + rand()%(N-num1);
            int *a;
            a = malloc(2*sizeof(int));
            a[0] = num1;
            a[1] = num2;
            qsort(chanels_info,K,sizeof(int*),cmp_pare);
            int **k;
            printf("HERR %d %d\n",num1,num2);
            k = bsearch(&a,chanels_info,K,sizeof(int*),cmp_pare);

            if (k)
            {
                (*k)[0] = -1;
                (*k)[1] = -1;
                printf("HERE\n");
                delete_connection(num1,num2);
                                printf("HERE\n");

                flag = 0;
            }
        }
        printf("DELETTED %d\n",i);
    }

    printf("Eightth step: adding water\n");
    for (int i=0;i<L;i++)
    {
        double x;
        int num;
        add_water(num = rand()%N,x=rand()%(WMAX - 1)+WMIN);
        printf("ADDED %lf to %d\n",x,num);
    }
    printf("\nEnd: measuring\n\n");
    for (int i=0;i<N;i++)
    {
        printf("Volume of %d pool is %lf\n",i,measure_volume(i));
    }
    free_pools();
}

