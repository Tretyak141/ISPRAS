#include <stdlib.h>
#include <stdio.h>

#define N 3
#define K 2
#define L 2
#define M 1
#define WMIN 1
#define WMAX 500

struct pool
{
    double v; 
    int num_of_gr;
    int *connected;
    int count;
    int start;
};



struct pool_group
{
    double volume;
    int *nums_in_gr;
    int st_pos;
    int count;
    int **conn_wth_grs;
    int con_start;
    int con_count;
    int *groups;
    int gr_st;
    int gr_count;
};

typedef struct pool pool;
typedef struct pool_group pool_group;

pool *pools;

pool_group *groups = NULL;
int group_start = 0;
int group_end = 0;

pool create()
{
    pool ret;
    ret.v = 0;
    ret.num_of_gr = -1;
    ret.connected = NULL;
    ret.count = 0;
    ret.start = 0;
    return ret;
}

pool_group fill(int num1,int num2)
{
    pool_group ret;
    ret.volume = pools[num1].v + pools[num2].v;
    ret.count = 2;
    ret.st_pos = 0;
    ret.nums_in_gr = malloc(2*sizeof(int));
    ret.nums_in_gr[0] = num1;
    ret.nums_in_gr[1] = num2;
    ret.conn_wth_grs = NULL;
    ret.con_start = 0;
    ret.con_count = 0;
    ret.groups = NULL;
    ret.gr_st = 0;
    ret.gr_count = 0;
    return ret;
}

void create_gr(int num1,int num2)
{
    group_end++;
    groups = realloc(groups,sizeof(pool_group)*group_end);
    groups[group_end-1] = fill(num1,num2);
}

int cmp(const void *num1,const void *num2)
{
    if ((*(int*)num1)>(*(int*)num2)) return 1;
    if ((*(int*)num1)<(*(int*)num2)) return -1;
    return 0;
}

int cmp1(const void *num1,const void *num2)
{
    if (((int*)(*(int**)num1))[2] > ((int*)(*(int**)num2))[2])
    {
        return 1;
    }
    if (((int*)(*(int**)num1))[2] == ((int*)(*(int**)num2))[2])
    {
        if (((int*)(*(int**)num1))[0] > ((int*)(*(int**)num2))[0]) return 1;
        if (((int*)(*(int**)num1))[0] == ((int*)(*(int**)num2))[0])
            if (((int*)(*(int**)num1))[1] > ((int*)(*(int**)num2))[1])
                return 1;
            if (((int*)(*(int**)num1))[1] < ((int*)(*(int**)num2))[1])
                return -1;
            return 0;
    }
    return -1;
}

/**
 * Creating channel between two pools
 * If they're in one group - just make relations between channels
 * If they're in differece groups - make relations between
 * these groups
 * If one pool has no group - adding it to group
 * Making group if two pools have no group
*/

void create_channel(int num1, int num2)
{
    if ((pools[num1].num_of_gr == -1) && (pools[num2].num_of_gr == -1))
    {
        create_gr(num1,num2);
        pools[num1].num_of_gr = group_end-1;
        pools[num2].num_of_gr = group_end-1;
    
        qsort(pools[num1].connected,pools[num1].count,sizeof(int),cmp);
        qsort(pools[num2].connected,pools[num2].count,sizeof(int),cmp);

        pools[num1].count++;
        pools[num1].connected = realloc(pools[num1].connected,sizeof(int)*pools[num1].count);
        pools[num2].count++;
        pools[num2].connected = realloc(pools[num2].connected,sizeof(int)*pools[num2].count);
    
        pools[num1].connected[pools[num1].count-1] = num2;
        pools[num2].connected[pools[num2].count-1] = num1;
    }
    else if (pools[num2].num_of_gr == -1)
    {
        pools[num2].num_of_gr = pools[num1].num_of_gr;

        qsort(pools[num1].connected,pools[num1].count,sizeof(int),cmp);
        qsort(pools[num2].connected,pools[num2].count,sizeof(int),cmp);
        
        pools[num1].count++;
        pools[num1].connected = realloc(pools[num1].connected,pools[num1].count);
        pools[num2].count++;
        pools[num2].connected = realloc(pools[num2].connected,pools[num2].count);
    
        pools[num1].connected[pools[num1].count-1] = num2;
        pools[num2].connected[pools[num2].count-1] = num1;

        groups[pools[num1].num_of_gr].count++;
        groups[pools[num1].num_of_gr].nums_in_gr = realloc(groups[pools[num1].num_of_gr].nums_in_gr,groups[pools[num1].num_of_gr].count);

        groups[pools[num1].num_of_gr].nums_in_gr[groups[pools[num1].num_of_gr].count - 1] = num2;

        groups[pools[num1].num_of_gr].volume += pools[num2].v;
    }
    else if (pools[num1].num_of_gr == -1)
    {
        pools[num1].num_of_gr = pools[num2].num_of_gr;
    
        qsort(pools[num1].connected,pools[num1].count,sizeof(int),cmp);
        qsort(pools[num2].connected,pools[num2].count,sizeof(int),cmp);
        pools[num1].count++;
        pools[num1].connected = realloc(pools[num1].connected,pools[num1].count);
        pools[num2].count++;
        pools[num2].connected = realloc(pools[num2].connected,pools[num2].count);
    
        pools[num1].connected[pools[num1].count-1] = num2;
        pools[num2].connected[pools[num2].count-1] = num1;
        

        
        groups[pools[num2].num_of_gr].count++;
        groups[pools[num2].num_of_gr].nums_in_gr = realloc(groups[pools[num2].num_of_gr].nums_in_gr,groups[pools[num2].num_of_gr].count);

        groups[pools[num2].num_of_gr].nums_in_gr[groups[pools[num2].num_of_gr].count - 1] = num1;
        groups[pools[num1].num_of_gr].volume += pools[num1].v;
    }
    else if (pools[num1].num_of_gr == pools[num2].num_of_gr)
    {
        if (!bsearch(&num2,pools[num1].connected,pools[num1].count,sizeof(int),cmp))
        {
            qsort(pools[num1].connected,pools[num1].count,sizeof(int),cmp);
            qsort(pools[num2].connected,pools[num2].count,sizeof(int),cmp);
            pools[num1].count++;
            pools[num1].connected = realloc(pools[num1].connected,pools[num1].count);
            pools[num2].count++;
            pools[num2].connected = realloc(pools[num2].connected,pools[num2].count);
        
            pools[num1].connected[pools[num1].count-1] = num2;
            pools[num2].connected[pools[num2].count-1] = num1;

        }
    }
    else
    {
        if (!bsearch(&num2,pools[num1].connected,pools[num1].count,sizeof(int),cmp))
        {
            qsort(pools[num1].connected,pools[num1].count,sizeof(int),cmp);
            qsort(pools[num2].connected,pools[num2].count,sizeof(int),cmp);
            pools[num1].count++;
            pools[num1].connected = realloc(pools[num1].connected,pools[num1].count);
            pools[num2].count++;
            pools[num2].connected = realloc(pools[num2].connected,pools[num2].count);
        
            pools[num1].connected[pools[num1].count-1] = num2;
            pools[num2].connected[pools[num2].count-1] = num1;

            
            qsort(groups[pools[num1].num_of_gr].groups,groups[pools[num1].num_of_gr].gr_count,sizeof(int),cmp);
            qsort(groups[pools[num2].num_of_gr].groups,groups[pools[num2].num_of_gr].gr_count,sizeof(int),cmp);

            if (!bsearch(&(pools[num2].num_of_gr),groups[pools[num1].num_of_gr].groups,groups[pools[num1].num_of_gr].gr_count,sizeof(int),cmp))
            {
                groups[pools[num1].num_of_gr].gr_count++;
                groups[pools[num1].num_of_gr].groups = realloc(groups[pools[num1].num_of_gr].groups,groups[pools[num1].num_of_gr].gr_count);
                groups[pools[num1].num_of_gr].groups[groups[pools[num1].num_of_gr].gr_count - 1] = pools[num2].num_of_gr;

                groups[pools[num2].num_of_gr].gr_count++;
                groups[pools[num2].num_of_gr].groups = realloc(groups[pools[num2].num_of_gr].groups,groups[pools[num2].num_of_gr].gr_count);
                groups[pools[num2].num_of_gr].groups[groups[pools[num2].num_of_gr].gr_count - 1] = pools[num1].num_of_gr;
            }
            groups[pools[num1].num_of_gr].con_count++;
            groups[pools[num1].num_of_gr].conn_wth_grs = realloc(groups[pools[num1].num_of_gr].conn_wth_grs,sizeof(int*)*groups[pools[num1].num_of_gr].con_count);
            groups[pools[num1].num_of_gr].conn_wth_grs[groups[pools[num1].num_of_gr].con_count - 1] = malloc(3*sizeof(int));
            groups[pools[num1].num_of_gr].conn_wth_grs[groups[pools[num1].num_of_gr].con_count - 1][0] = num1;
            groups[pools[num1].num_of_gr].conn_wth_grs[groups[pools[num1].num_of_gr].con_count - 1][1] = num2;
            groups[pools[num1].num_of_gr].conn_wth_grs[groups[pools[num1].num_of_gr].con_count - 1][2] = pools[num2].num_of_gr;

            groups[pools[num2].num_of_gr].con_count++;
            groups[pools[num2].num_of_gr].conn_wth_grs = realloc(groups[pools[num2].num_of_gr].conn_wth_grs,sizeof(int*)*groups[pools[num2].num_of_gr].con_count);
            groups[pools[num2].num_of_gr].conn_wth_grs[groups[pools[num2].num_of_gr].con_count - 1] = malloc(3*sizeof(int));
            groups[pools[num2].num_of_gr].conn_wth_grs[groups[pools[num2].num_of_gr].con_count - 1][0] = num1;
            groups[pools[num2].num_of_gr].conn_wth_grs[groups[pools[num2].num_of_gr].con_count - 1][1] = num2;
            groups[pools[num2].num_of_gr].conn_wth_grs[groups[pools[num2].num_of_gr].con_count - 1][2] = pools[num1].num_of_gr;


        }
    }
}

double group_volume(int *checked,int *len_obh,int current)
{
    if (!checked)
    {
        checked = malloc(sizeof(int));
        *len_obh = 1;
        checked[0] = current;
    }
    double ans = groups[current].volume;
    qsort(checked,*len_obh,sizeof(int),cmp);

    int flag = 0;
    for (int i=groups[current].gr_st;i<groups[current].gr_count;i++)
    {
        if (groups[current].groups[i]!=-1)
        {
            int *found = bsearch(&groups[current].groups[i],checked,*len_obh,sizeof(int),cmp);
            if (!found)
            {
                flag = 1;
                break;
            }
        }
    }

    if (flag == 0) 
    {
        return ans;
    }

    
    for (int i=groups[current].gr_st;i<groups[current].gr_count;i++)
    {
        if (groups[current].groups[i]!=-1)
        {
            int *found = bsearch(&groups[current].groups[i],checked,*len_obh,sizeof(int),cmp);
            if (!found)
            {
                (*len_obh) ++;
                checked = realloc(checked,sizeof(int)*(*len_obh));
                checked[*len_obh - 1] = groups[current].groups[i];
                qsort(checked,*len_obh,sizeof(int),cmp);
                ans += group_volume(checked,len_obh,groups[current].groups[i]);
            }
        }
    }
    return ans;
}

int group_counter(int *checked,int *len_obh,int current)
{
    if (!checked)
    {
        checked = malloc(sizeof(int));
        *len_obh = 1;
        checked[0] = current;
    }
    int ans = groups[current].count - groups[current].st_pos;
    qsort(checked,*len_obh,sizeof(int),cmp);

    int flag = 0;
    for (int i=groups[current].gr_st;i<groups[current].gr_count;i++)
    {
        if (groups[current].groups[i]!=-1)
        {
            int *found = bsearch(&groups[current].groups[i],checked,*len_obh,sizeof(int),cmp);
            if (!found)
            {
                flag = 1;
                break;
            }
        }
    }

    if (flag == 0) 
    {
        return ans;
    }

    
    for (int i=groups[current].gr_st;i<groups[current].gr_count;i++)
    {
        if (groups[current].groups[i]!=-1)
        {
            int *found = bsearch(&groups[current].groups[i],checked,*len_obh,sizeof(int),cmp);
            if (!found)
            {
                (*len_obh) ++;
                checked = realloc(checked,sizeof(int)*(*len_obh));
                checked[*len_obh - 1] = groups[current].groups[i];
                qsort(checked,*len_obh,sizeof(int),cmp);
                ans += group_counter(checked,len_obh,groups[current].groups[i]);
            }
        }
    }
    return ans;
}

/**
 * Measure volume of pool using information 
 * about connected groups
*/
double measure_volume(int num1)
{
    if (pools[num1].num_of_gr == -1) return pools[num1].v;
    int *len_obh1;
    int *len_obh2;
    len_obh1 = malloc(sizeof(int));
    len_obh1[0] = 0;
    len_obh2 = malloc(sizeof(int));
    len_obh2[0] = 0;

    int *checked1 = NULL;
    int *checked2 = NULL;
    double ans = group_volume(checked1,len_obh1,pools[num1].num_of_gr)/group_counter(checked2,len_obh2,pools[num1].num_of_gr);
    free(checked1);
    free(checked2);
    return ans;
}

/**
 * Delete connection between two pools
 * If there're connection in one group - check changing and delete
 * pool from group if it was just one connection between pool and group
 * If there're connections between two groups - delete this connection,
 * and, if it was just one connection, delete relations between groups
*/

void delete_connection(int num1,int num2)
{
    qsort(pools[num1].connected,pools[num1].count,sizeof(int),cmp);
    qsort(pools[num2].connected,pools[num2].count,sizeof(int),cmp);
    int *found;
    if (!(found = (int*)bsearch(&num1,pools[num2].connected,pools[num2].count,sizeof(int),cmp)))
    {
        return;
    }
    *found = -1;
    found = (int*)bsearch(&num2,pools[num1].connected,pools[num1].count,sizeof(int),cmp);
    *found = -1;
    pools[num1].start ++;
    pools[num2].start ++;
    qsort(groups[pools[num1].num_of_gr].conn_wth_grs,groups[pools[num1].num_of_gr].con_count,sizeof(int*),cmp1);
    qsort(groups[pools[num2].num_of_gr].conn_wth_grs,groups[pools[num2].num_of_gr].con_count,sizeof(int*),cmp1);

    int *a = malloc(sizeof(int)*3);
    a[0] = num1;
    a[1] = num2;
    a[2] = pools[num2].num_of_gr;
    /**
     * Situation where groups are difference
    */

    int **find;
    find = bsearch(&a,groups[pools[num1].num_of_gr].conn_wth_grs,groups[pools[num1].num_of_gr].con_count,sizeof(int*),cmp1);
    if (find)
    {

        (*find)[2] = -1;
        groups[pools[num1].num_of_gr].con_start++;
    }

    int b[3] = {num1,num2,pools[num1].num_of_gr};

    find = bsearch(&b,groups[pools[num2].num_of_gr].conn_wth_grs,groups[pools[num2].num_of_gr].con_count,sizeof(int*),cmp1);
    if (find)
    {
        if ((found+1)[2] != found[2])
        {
            if (found>groups[pools[num2].num_of_gr].conn_wth_grs)
            {
                if ((found-1)[2]!=found[2])
                {
                    for (int i=groups[pools[num2].num_of_gr].gr_st;groups[pools[num2].num_of_gr].count;i++)
                    {
                        if (groups[pools[num2].num_of_gr].groups[i]==pools[num1].num_of_gr)
                        {
                            groups[pools[num2].num_of_gr].groups[i] = -1;
                        }
                    }
                    groups[pools[num2].num_of_gr].gr_st ++;

                    for (int i=groups[pools[num1].num_of_gr].gr_st;groups[pools[num1].num_of_gr].count;i++)
                    {
                        if (groups[pools[num1].num_of_gr].groups[i]==pools[num2].num_of_gr)
                        {
                            groups[pools[num1].num_of_gr].groups[i] = -1;
                        }
                    }
                    groups[pools[num1].num_of_gr].gr_st ++;
                }
            }
            else
            {

                for (int i=groups[pools[num2].num_of_gr].gr_st;groups[pools[num2].num_of_gr].count;i++)
                {
                    if (groups[pools[num2].num_of_gr].groups[i]==pools[num1].num_of_gr)
                    {
                        groups[pools[num2].num_of_gr].groups[i] = -1;
                    }
                }
                groups[pools[num2].num_of_gr].gr_st ++;

                for (int i=groups[pools[num1].num_of_gr].gr_st;groups[pools[num1].num_of_gr].count;i++)
                {
                    if (groups[pools[num1].num_of_gr].groups[i]==pools[num2].num_of_gr)
                    {
                        groups[pools[num1].num_of_gr].groups[i] = -1;
                    }
                }
                groups[pools[num1].num_of_gr].gr_st ++;

            }
        }
        found[2] = -1;
        
        groups[pools[num2].num_of_gr].con_start++;

        return;
    }


    /**
     * Same groups
    */

    qsort(pools[num1].connected,pools[num1].count,sizeof(int),cmp);
    qsort(pools[num2].connected,pools[num2].count,sizeof(int),cmp);

    qsort(groups[pools[num1].num_of_gr].nums_in_gr,groups[pools[num1].num_of_gr].count,sizeof(int),cmp);
    qsort(groups[pools[num2].num_of_gr].nums_in_gr,groups[pools[num2].num_of_gr].count,sizeof(int),cmp);
    
    int flag = 0;
    for (int i=pools[num1].start;i<pools[num1].count;i++)
    {
        if ((pools[num1].connected[i]!=-1) && bsearch(&pools[num1].connected[i],groups[pools[num1].num_of_gr].nums_in_gr,groups[pools[num1].num_of_gr].count,sizeof(int),cmp))
        {
            flag = 1;
            break;
        }
    }
    if (!flag)
    {
        int gr_flag = 0;
        int save = pools[num1].num_of_gr;
        for (int i=pools[num1].start;i<pools[num1].count;i++)
        {
            if (pools[num1].connected[i]!=-1)
            {
            /**
             * Distribution empty pool to the group
             * Num of group: min num of connected groups
             */                    
                pools[num1].num_of_gr = pools[pools[num1].connected[i]].num_of_gr;
                groups[pools[pools[num1].connected[i]].num_of_gr].count++;
                groups[pools[pools[num1].connected[i]].num_of_gr].nums_in_gr = realloc(groups[pools[pools[num1].connected[i]].num_of_gr].nums_in_gr,sizeof(int)*groups[pools[pools[num1].connected[i]].num_of_gr].count); 
                groups[pools[pools[num1].connected[i]].num_of_gr].groups[groups[pools[pools[num1].connected[i]].num_of_gr].count - 1] = num1;
                groups[pools[num1].num_of_gr].volume += pools[num1].v;
                gr_flag = 1;
                break;
            }
        }
        pools[num1].start++;

        if (!gr_flag)
{            int *checked = NULL;
            int *num;
            *num = 0;
            groups[save].volume -= pools[num1].v;
            groups[save].st_pos ++;
            pools[num1].v = group_volume(checked,num,save)/group_counter(checked,num,save);
            free(checked);
            printf("HEREs\n");
            pools[num1].num_of_gr = -1;
            
            int *f;
            qsort(groups[save].nums_in_gr,groups[save].count,sizeof(int),cmp);
            f = bsearch(&num1,groups[save].nums_in_gr,groups[save].count,sizeof(int),cmp);
            if (f)
            {
                *f = -1;
            
            }
            printf("HEREsss\n");

        }
    }

    flag = 0;
    for (int i=pools[num2].start;i<pools[num2].count;i++)
    {
        if ((pools[num2].connected[i]!=-1) && bsearch(&pools[num2].connected[i],groups[pools[num2].num_of_gr].nums_in_gr,groups[pools[num2].num_of_gr].count,sizeof(int),cmp))
        {
            flag = 1;
            break;
        }
    }
    if (!flag)
    {
        int gr_flag = 0;
        int save = pools[num2].num_of_gr;

        for (int i=pools[num2].start;i<pools[num2].count;i++)
        {
            if (pools[num2].connected[i]!=-1)
            {
                
                pools[num2].num_of_gr = pools[pools[num2].connected[i]].num_of_gr;
                groups[pools[pools[num2].connected[i]].num_of_gr].count++;
                groups[pools[pools[num2].connected[i]].num_of_gr].nums_in_gr = realloc(groups[pools[pools[num2].connected[i]].num_of_gr].nums_in_gr,sizeof(int)*groups[pools[pools[num2].connected[i]].num_of_gr].count); 
                groups[pools[pools[num2].connected[i]].num_of_gr].groups[groups[pools[pools[num2].connected[i]].num_of_gr].count - 1] = num2;
                groups[pools[num2].num_of_gr].volume+=pools[num2].v;
                gr_flag = 1;
                break;
            }
        }
        if (!gr_flag)
        {
            pools[num2].num_of_gr = -1;
        }
        int *f;
        f = bsearch(&num2,groups[save].nums_in_gr,groups[save].count,sizeof(int),cmp);
        if (f)
        {
            *f = -1;
        }
        int *checked = NULL;
        int *num;
        *num = 0;
        pools[num2].v = group_volume(checked,num,save)/group_counter(checked,num,save);
        groups[save].volume -= pools[num2].v;
        groups[save].st_pos ++;

        free(checked);
    }
    
}

void add_water(int num1,double volume)
{
    pools[num1].v += volume;
    if (pools[num1].num_of_gr != -1)
        groups[pools[num1].num_of_gr].volume += volume;
}

void free_pools()
{
    for (int i=0;i<N;i++)
    {
        free(pools[i].connected);
    }
    for (int i=0;i<group_end;i++)
    {
        free(groups[i].groups);
        free(groups[i].nums_in_gr);
        
        for (int j=0;j<groups[i].con_count;j++)
        {
            free(groups[i].conn_wth_grs[j]);
        }
        free(groups[i].conn_wth_grs);
    }
    free(groups);
}