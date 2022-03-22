// Wyatt LeMaster COMP7500 AUBATCH
// Final verison 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#define CMD_BUF_SIZE 50 /* The size of the command queue */


// global variabels used for keeping track of queue
u_int count;
u_int buf_head;
u_int buf_tail;

// global variables that allow setting of policy and marking termination
u_int policy;
int terminate; //-FLAG



pthread_mutex_t cmd_queue_lock;  /* Lock for critical sections */
pthread_cond_t queue_not_full; /* Condition variable for buf_not_full */
pthread_cond_t queue_not_empty; /* Condition variable for buf_not_empty */



/* ####################  Global structure def ############*/
struct job_info     
{
    int process_num;
    double exec_time;  
    double est_cpu_time;
    int priority;
    double start_time;
    double finish_time;
    struct timeval wait_start, wait_end;
    int wait;
    double arrival_time;
    char* name[100];
    char progress[5];

    
};


//d3
struct performance_info
{
    double total_cpu_time;
    long int total_waiting_time;
    double turnaround_time;
    int num_jobs;
    double throughput;
};


//d4
struct workload_info
{
    int num_of_jobs;
    int arrival_rate; 
    double min_cpu_time;
    double max_cpu_time;
    int min_priority_level;
    int max_priority_level;
};


/*########## Data structures END ##########*/


struct job_info queue[CMD_BUF_SIZE];

struct job_info completed_job_queue[1000];

struct performance_info batch_totals;
