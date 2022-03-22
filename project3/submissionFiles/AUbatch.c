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
#include "global.h"
#include "command_parser.h"
#include "scheduling_module.h"
#include "dispatching_module.h"
#define CMD_BUF_SIZE 50 /* The size of the command queue */
#define FCFS  1
#define SJF  2
#define PRIORITY  3


/* COMMENT BLOCK 
# 
#    AUbatch.c by Wyatt LeMaster
#    see below for references used.
#
#    This program runs batch processing jobs.
#    In order to do this two threads are used.
#    A scheduling thread and a dispatching thread.
#    the jobs can be submitted individually
#   or the user can run as bench mark.
#
#   the bench mark takes the form of
#   test <benchmark> <policy> <num_of_jobs> 
#   <arrival_rate> <priority_levels> <min_CPU_time> <max_CPU_time>
#
#
#    There are three different scheduling 
#   polices you can use for the job execution
#   First come first serve, Shortest job first, Priority.
#    with FCFS being the default.
#
#   when exiting the program you will be able to view the preformance statistics 
#
#
#
#
#
#  How to compile: 
#  with make file you can simply type make
#   then run the resulting AUbatch program
# 
#  if not using make:
#   gcc -g -w -c command_parser.c
#	gcc -g -w -c dispatching_module.c
#	gcc -g -w -c scheduling_module.c
#	gcc -g -w -c AUbatch-temp.c -lpthread
#	gcc -o batchtest AUbatch-temp.o command_parser.o scheduling_module.o dispatching_module.o -lpthread -lm
#
#  
# 
#
#
#
#
# References:
# aubatch_sample.c Written by Xiao Qin 
#
#  process.c
#  https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?
#  answer=1044654269&id=1043284392
#  Modified by Xiao Qin
#
# random number generation https://tinyurl.com/28mwxndt# 
#

#
#
#
#
#
*/


// setting terminate flag to false
terminate = 0;

/*########## start of functions ##########*/
//6
void performance_measurements();

//7 
void automated_performance_evaluation();


/*########## End of functions ##########*/




int main( int argc, char *argv[])
{


    policy = FCFS;
    print_menu();

    /* FROM aubtach_sample.c Written by Xiao Qin */ 

    pthread_t command_thread, executor_thread; /* Two concurrent threads */
    char *message1 = "Command Thread";
    char *message2 = "Executor Thread";
    int  iret1, iret2;

    /* Initilize count, two buffer pointers */
    count = 0; 
    buf_head = 0;  
    buf_tail = 0; 
    /* Create two independent threads:command and executors */
    iret1 = pthread_create(&command_thread, NULL, scheduling_module,  NULL);
    iret2 = pthread_create(&executor_thread, NULL, dispatching_module, NULL);
   

    /* Initialize the lock the two condition variables */
    pthread_mutex_init(&cmd_queue_lock, NULL);
    pthread_cond_init(&queue_not_full, NULL);
    pthread_cond_init(&queue_not_empty, NULL);
     
    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */
    pthread_join(command_thread, NULL);
    pthread_join(executor_thread, NULL); 


printf("\n\nThank you for using AUbatch\n\n");
return 0;
}




/*########## start of functions ##########*/




void performance_measurements()
{

/*
calculates the performance measurments from data gathered during run time.
*/

int num_jobs = batch_totals.num_jobs;

double total_waiting_time = batch_totals.total_waiting_time / 1000;

double total_cpu_time = batch_totals.total_cpu_time / 1000;

double total_time = total_cpu_time;

double Average_cpu_time = total_cpu_time /  num_jobs;

double Average_turn_around_time = (total_cpu_time + total_waiting_time) / num_jobs;


double Average_waiting_time = batch_totals.total_waiting_time / num_jobs;


double throughput =  total_time / num_jobs;


// prints calculated information
printf("\n\nTotal number of job submitted: %d\n",num_jobs);

printf("Average turnaround time: %.2f seconds\n", Average_turn_around_time);

printf("Average CPU time:        %.2f seconds\n",Average_cpu_time);

printf("Average wait time:       %.2f seconds\n",Average_waiting_time / 1000);

printf("throughput:              %.3f No./second\n",throughput );


}


/*########## End of functions ##########*/


void automated_performance_evaluation(char *args[])
{

    /*
    takes arguments and builds a benchmark using them

    has two options for testing.
    1. test load - this loads a hard coded queue for consistent testing

    2. test <benchmark> <policy> <num_of_jobs> <priority_levels>
            <min_CPU_time> <max_CPU_time>

        there is only one benchmark to be run named:
        bench/benchmark/BENCH/BENCHMARK you assign it the values
        for testing.

        an example execution would be <test benchmark priority 5 3 1 5>
    */




  int choice,  num_jobs,  pri_levels,  wait_start;
  double min_cpu,  max_cpu, arrival_rate;

  if(strcmp(args[1], "load") ==0 )
   {


    /* Builds jobs for testing */
     struct job_info test_batch[5];

     snprintf(test_batch[0].name, sizeof(test_batch[0].name), "%s", "process");
     snprintf(test_batch[1].name, sizeof(test_batch[1].name), "%s", "process");
     snprintf(test_batch[2].name, sizeof(test_batch[2].name), "%s", "process");
     snprintf(test_batch[3].name, sizeof(test_batch[3].name), "%s", "process");
     snprintf(test_batch[4].name, sizeof(test_batch[4].name), "%s", "process");

     test_batch[0].arrival_time = 0; 
     test_batch[1].arrival_time = 0.2;
     test_batch[2].arrival_time = 0.6;
     test_batch[3].arrival_time = 0.4;
     test_batch[4].arrival_time = 0.8;
    
     test_batch[0].priority = 1;
     test_batch[1].priority = 4;
     test_batch[2].priority = 2;
     test_batch[3].priority = 3;
     test_batch[4].priority = 5;

     test_batch[0].est_cpu_time = 4.4;
     test_batch[1].est_cpu_time = .5;
     test_batch[2].est_cpu_time = 6;
     test_batch[3].est_cpu_time = 1;
     test_batch[4].est_cpu_time = 2;


     gettimeofday(&test_batch[0].wait_start, NULL);
     gettimeofday(&test_batch[1].wait_start, NULL);
     gettimeofday(&test_batch[2].wait_start, NULL);
     gettimeofday(&test_batch[3].wait_start, NULL);
     gettimeofday(&test_batch[4].wait_start, NULL);

     strcpy(test_batch[0].progress,"");
     strcpy(test_batch[1].progress,"");
     strcpy(test_batch[2].progress,"");
     strcpy(test_batch[3].progress,"");
     strcpy(test_batch[4].progress,"");

     test_batch[0].process_num = 1;
     test_batch[1].process_num = 2;
     test_batch[2].process_num = 3;
     test_batch[3].process_num = 4;
     test_batch[4].process_num = 5;
    
    
     int i = 0;

    /*adds built jobs to the queue */
     for( i = 0; i < 5; i++)
        {

            queue[buf_head] = test_batch[i];
            buf_head++;
            count++;
            if (buf_head == CMD_BUF_SIZE)
            buf_head = 0;
        }

    }

    else 
    {
        int error = 0; // tells the rest of benchmark not to run


        // checks to ensure benchmark name is correct
        if (strcmp(args[1], "benchmark") == 1){error = 1;}
        else if (strcmp(args[1], "BENCHMARK") == 1){error = 1;}
        else if (strcmp(args[1], "bench") == 1){error = 1;}
        else if (strcmp(args[1], "BENCH") == 1){error = 1;}
        
       
        // works for all capitalized or lower case policy.
        if (strcmp(args[2], "fcfs") == 0){ policy = FCFS; }
        else if (strcmp(args[2], "FCFS") == 0){ policy = FCFS;}

        else if (strcmp(args[2], "priority") == 0){policy = PRIORITY;}
        else if (strcmp(args[2], "PRIORITY") == 0){policy = PRIORITY;}

        else if(strcmp(args[2], "sjf") == 0){ policy = SJF; }
        else if(strcmp(args[2], "SJF") == 0){ policy = SJF; }

        // gives error if policy choice is incorrect.
        else 
        {
            printf("ERROR! \nPlease check your policy choice\n <fcfs> <priority> <sjf>\n");
            error = 1;
        }


        if(error == 0 )
        {

          // converts given job info into int and doubles allowing calculations 
          num_jobs = atoi(args[3]);
          arrival_rate = atof(args[4]);
          pri_levels = atoi(args[5]);
          //sscanf(args[6], "%lf", &min_cpu); 
          min_cpu = 1;
          sscanf(args[7], "%lf", &max_cpu);
        
        
          double random_cpu_time, random_arrival_rate;

          // initialize rand 
          srand ( time ( NULL));


          
          int i = 0;
          for( i = 0; i < num_jobs; i++)
          {
                
             //reference https://stackoverflow.com/questions/33058848/generate-a-random-double-between-1-and-1
             // generates random floating number between min and max run times
             double range = (max_cpu - min_cpu); 
             double div = RAND_MAX / range;
             random_cpu_time = min_cpu + (rand() / div); //generates random cpu time
             queue[buf_head].est_cpu_time = random_cpu_time; // sets cpu time



             
             div = RAND_MAX / arrival_rate; // generates random arrival times
             random_arrival_rate = (rand() / div);
             queue[buf_head].arrival_time = random_arrival_rate; // cpu time



             queue[buf_head].priority = rand() % pri_levels+1; // priority

             snprintf(queue[buf_head].name, sizeof(queue[buf_head].name), "%s", "process"); //name

             gettimeofday(&queue[buf_head].wait_start, NULL); // waiting time

             strcpy(queue[buf_head].progress,""); // progress

             queue[buf_head].process_num = count;
             // updates queue statistics 
             buf_head++;
             count++;
             if (buf_head == CMD_BUF_SIZE)
                buf_head = 0;

            }

        }

        // prints if something is entered incorrectly
        else 
        {
         printf("\n**please enter the correct arguments**");
         printf("\ntest <benchmark> <policy> <num_of_jobs> <arrival_rate>");
         printf(" <priority_levels <min_CPU_time> <max_CPU_time> \n\n");

        }
    }

}
