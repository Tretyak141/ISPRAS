#ifndef _structs_h_
#define _structs_h_

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

pool create();

pool_group fill(int num1,int num2);

extern pool *pools;

extern pool_group *groups;
extern int group_start;
extern int group_end;

void create_gr(int num1,int num2);

int cmp(const void *num1,const void *num2);

int cmp1(const void *num1,const void *num2);

/**
 * Creating channel between two pools
 * If they're in one group - just make relations between channels
 * If they're in differece groups - make relations between
 * these groups
 * If one pool has no group - adding it to group
 * Making group if two pools have no group
*/

void create_channel(int num1, int num2);

double group_volume(int *checked,int *len_obh,int current);

int group_counter(int *checked,int *len_obh,int current);

/**
 * Measure volume of pool using information 
 * about connected groups
*/
double measure_volume(int num1);

/**
 * Delete connection between two pools
 * If there're connection in one group - check changing and delete
 * pool from group if it was just one connection between pool and group
 * If there're connections between two groups - delete this connection,
 * and, if it was just one connection, delete relations between groups
*/

void delete_connection(int num1,int num2);
void add_water(int num1,double volume);
void free_pools();
#endif