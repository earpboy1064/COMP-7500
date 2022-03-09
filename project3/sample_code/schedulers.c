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

#define FCFS  1
#define SJF  2
#define PRIORITY  3

u_int policy = FCFS;

struct job_info
{
    char* name;
    int argv;  // this might be the time to run
    double est_cpu_time;
    int priority;
    double start_time;
    double finish_time;
    int wait;

    // can add arrival_time if needed
};


struct job_info *queue[];
int n = 0;
int tail = 0;


void add_job(struct job_info *job)
{
//printf("job name = %s", job->name);

queue[n] = job;
n++;



//this code needs to be finished. it needs to shift array over after insertion.
//int i , j;
/*
if (policy == SJF)
{

    for (i = 0; i < n; i++)
    {
        if (job->est_cpu_time < queue[i]->est_cpu_time)
        {
            struct job_info temp_one, temp_two;
            temp_one = queue[i];
            queue[i] = job;

            for (j = i; j < n - 1; j++)
            {
                
                temp_two = queue[i+1]
                queue[i+1] = temp_one

                
            }
        }
    
    }
*/
}



printf("policy = %d \n\n", policy);
};


void bubblesort()
{

        
       // Sorting using bubble sort
        int i = 0;
        int j = 0;
        double job_one, job_two;
        for (i = 0; i < n - 1; i++)
        {
            for (j = 0; j < n-i-1; j++)
            {
                if (policy == SJF)
                {
                    job_one = queue[j]->est_cpu_time;
                    job_two =  queue[j+1]->est_cpu_time;  
                }

                else if(policy == PRIORITY) // job assignments swapped to sort in reverse order
                {
                    job_two = queue[j]->priority;
                    job_one =  queue[j+1]->priority;
                }

                if(job_one > job_two)
                {
                    //swap elements
                    struct job_info *temp;
                    temp = queue[j];
                    queue[j] = queue[j+1];
                    queue[j+1] = temp;
                }
            }  
         }
    


}



int main( int argc, char *argv[])
{


struct job_info jobOne; 

struct job_info jobTwo; 

struct job_info jobThree; 

jobOne.name = "jobOne";
jobOne.est_cpu_time = 1.1;
jobOne.priority = 3;


jobTwo.name = "jobTwo";
jobTwo.est_cpu_time = 4.2;
jobTwo.priority = 5;


jobThree.name = "jobThree";
jobThree.est_cpu_time = 0.4;
jobThree.priority = 4;



add_job(&jobOne);
add_job(&jobTwo);
add_job(&jobThree);
int i = 0;

bubblesort();
printf("\n*****First Come First Serve*****\n");
for ( i = 0; i < n; i ++)
{
printf("\njob name: %s\nestimated cpu time: %f\nPriority: %d\nnumber of jobs in queue: %d\n", queue[i]->name, queue[i]->est_cpu_time ,queue[i]->priority,n);
}

policy = SJF;
bubblesort();
printf("\n*****SJF*****\n");

for ( i = 0; i < n; i ++)
{
printf("\njob name: %s\nestimated cpu time: %f\nPriority: %d\nnumber of jobs in queue: %d\n", queue[i]->name, queue[i]->est_cpu_time, queue[i]->priority,n);
}

policy = PRIORITY;
bubblesort();
printf("\n*****PRIORITY*****\n");

for ( i = 0; i < n; i ++)
{
printf("\njob name: %s\nestimated cpu time: %f\nPriority: %d\nnumber of jobs in queue: %d\n", queue[i]->name, queue[i]->est_cpu_time, queue[i]->priority ,n);
}


}