#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>



struct job_info
{
    char* name;
    int argv;  // this might be the time to run
    double estimated_cpu_time;
    int priority;
    double start_time;
    double finish_time;
    int wait;

    // can add arrival_time if needed
};


struct job_info *queue[];

void FCFS(struct job_info *job)
{


//printf("job name = %s", job->name);

queue[0] = job;




}



int main( int argc, char *argv[])
{
struct job_info job; 

job.name = "job One";

FCFS(&job);

printf("job from queue: %s", queue[0]->name);

//printf("job name = %s", job.name);
};