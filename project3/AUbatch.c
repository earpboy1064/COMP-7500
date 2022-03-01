#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// struct prototypes 



//d1
struct job_info_queue
{
    char job_name[100];
    int argv;  // this might be the time to run
    double estimated_cpu_time;
    int priority;
    double start_time;
    double finish_time;
    int wait;

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
    double total_waiting_time;
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



/*########## start of functions ##########*/
//1
void scheduing_module(struct scheduling_policy policy, struct workload_info workload, struct job_info_queue job_info);

//2
void dispatching_module();

//3
void commandline_parser();

//4 
void display_job_queue();

//5
void help();

//6
void performance_measurements();

//7 
void automated_performance_evaluation();
/*########## End of functions ##########*/


/*########## Data structures ##########*/




int main( int argc, char *argv[])
{
struct job_info_queue job_queue;
job_queue.priority = 2; 
printf("%d",job_queue.priority);

printf("\ntesting git\n\n");
return 0;
}




/*########## start of functions ##########*/
//1

void scheduing_module(struct scheduling_policy policy, struct workload_info workload, struct job_info_queue job_info)
{

//if (policy == FCFS){};


//if (policy == SJF){};


//if (policy == PRIORITY){};


// we create the queue here ^

// we then send the queue to the job_information_queue here v


}

//2
void dispatching_module(){};

//3
void commandline_parser(){};

//4 
void display_job_queue()
{
// just printing the job queue and information

}

//5
void help(){}

//6
void performance_measurements(){}

//7 
void automated_performance_evaluation(){}


/*########## End of functions ##########*/
