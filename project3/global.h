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




u_int count;
u_int buf_head;
u_int buf_tail;

#define CMD_BUF_SIZE 10 /* The size of the command queue */


u_int policy;
int terminate; //-FLAG



pthread_mutex_t cmd_queue_lock;  /* Lock for critical sections */
pthread_cond_t queue_not_full; /* Condition variable for buf_not_full */
pthread_cond_t queue_not_empty; /* Condition variable for buf_not_empty */


struct job_info      //maybe move this to a different file.
{
    double exec_time;  // this might be the time to run
    double est_cpu_time;
    int priority;
    double start_time;
    double finish_time;
    struct timeval wait_start, wait_end;
    int wait;
    double arrival_time;
    char* name[100];
    char progress[5];

    // can add arrival_time if needed
};



//d2
struct help_info
{
    char command[100];
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
    int arrival_rate; // testing interval 
    double min_cpu_time;
    double max_cpu_time;
    int min_priority_level;
    int max_priority_level;
};

//d5 
struct scheduling_policy
{
    //enum polices {FCFS, SJF, PRIORITY};
};

/*########## Data structures END ##########*/


struct job_info queue[CMD_BUF_SIZE];
struct job_info completed_job_queue[1000];

