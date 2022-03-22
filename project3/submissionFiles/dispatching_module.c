
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
#include "dispatching_module.h"
#include "global.h"

/*
COMMENT BLOCK
This handles the execution and removal of jobs from the queue.
it also contains the sorting algorithm used to ensure job order is correct.

*/


void dispatching_module()
{
    
    while(terminate == 0){

        //######## queue handling and termination check ########//

        /* lock and unlock for the shared process queue */
        pthread_mutex_lock(&cmd_queue_lock);

        while ((count == 0) && (terminate ==0)) {
            pthread_cond_wait(&queue_not_empty, &cmd_queue_lock);
        }


        // checks if terminate changed while in pthread_cond wait
        // if so it breaks the while loop allowing thread to exit cleanly.
        if (terminate ==1)
        break;


        // ensures that the queue is sorted correctly
        bubble_sort();



        //######## queue handling and termination check ########//
    
        // ******* Building argument list to be passed to argv **********
        // this converts from double to string to be passed to argv
        char process_name[100];
        strcpy(process_name, queue[buf_tail].name);
       
        double temp_double = queue[buf_tail].est_cpu_time;
        char arg_convert[5];
        // only converts single decimal place
        sprintf(arg_convert,"%.1f",queue[buf_tail].est_cpu_time);

        char *my_args[5]; 
        my_args[0] = arg_convert;
        my_args[1] = NULL;
        // ******* Building argument list to be passed to argv **********



        strcpy(queue[buf_tail].progress,"Run"); // updating job information

        //Reference
        //https://stackoverflow.com/questions/9871071/why-c-clock-returns-0
        gettimeofday(&queue[buf_tail].wait_end, NULL);
        
        // *************** Calculating wait *****************************************
        long mtime, secs, usecs;    
        secs  = queue[buf_tail].wait_end.tv_sec  - queue[buf_tail].wait_start.tv_sec;
        usecs = queue[buf_tail].wait_end.tv_usec - queue[buf_tail].wait_start.tv_usec;
        mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
        //printf("milli waited = %ld", mtime);
        batch_totals.total_waiting_time += mtime;
        // *************** Calculating wait *****************************************





        //starting runtime for process
        struct timeval exec_start, exec_end;
        gettimeofday(&exec_start, NULL);
        //

        pid_t pid; // holds pid of child

        switch ((pid = fork()))
        {
            case -1:
                /* Fork() has failed */
                perror("fork");
                break;
            case 0:
                /* This is processed by the child */
                execv(process_name, my_args);               
                puts("Uh oh! If this prints, execv() must have failed");
                exit(EXIT_FAILURE);
                break;
            default:
                break;
        }

        


        // going ahead and unlocking the queue while process is running.
        pthread_cond_signal(&queue_not_full);
        /* Unlock the shared command queue */
        pthread_mutex_unlock(&cmd_queue_lock);

        // waiting for process to return.
        int w, status;
        w = waitpid(pid, NULL, 0 );
       

        //******* PERFORMANCE INFO UPDATED ***************
        // calculating the time spent running process

        gettimeofday(&exec_end, NULL);
        secs  = exec_end.tv_sec  - exec_start.tv_sec;
        usecs = exec_end.tv_usec - exec_start.tv_usec;
        mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
        
        batch_totals.total_cpu_time += mtime;
        batch_totals.num_jobs++;
        //printf(" exec time %.1f \n\n", batch_totals.total_cpu_time);
        
        //******* PERFORMANCE INFO UPDATED ***************
        
       


        // updating tail location
        count--; 
        buf_tail++;
         if (buf_tail == CMD_BUF_SIZE)
            buf_tail = 0;

      
    } /* end for */

    
}; 





void bubble_sort()
{
    /*
    Using the bubble sort algorithm this will reorder the
    circular queue as long as count > 2
    this is to prevent it getting stuck in the loops
    also you do not need to order a list < 2.
    */

    if(count > 2 )
    {
       // Sorting using bubble sort
        int i = 0;
        int j = 0;
        double job_one, job_two;

        int first_pos = buf_tail;
        int second_pos = buf_tail + 1;
        for (i = 0; i < count - 1; i++)
        {
            int first_pos = buf_tail;
            int second_pos = buf_tail + 1;

            for (j = 0; j < count-i-1; j++)
            {
                if (first_pos == CMD_BUF_SIZE)
                first_pos = 0;


                if (second_pos == CMD_BUF_SIZE)
                second_pos = 0;

                if (policy == SJF)
                {
                    job_one = queue[first_pos].est_cpu_time;
                    job_two =  queue[second_pos].est_cpu_time;  
                }

                else if(policy == PRIORITY) 
                {
                    job_one = queue[first_pos].priority;
                    job_two =  queue[second_pos].priority;
                }

                 else if(policy == FCFS) 
                {
                    job_one = queue[first_pos].arrival_time;
                    job_two =  queue[second_pos].arrival_time;
                }

                if(job_one > job_two)
                {
                    //swap elements
                    struct job_info temp;
                    temp = queue[first_pos];
                    queue[first_pos] = queue[second_pos];
                    queue[second_pos] = temp;

                }

                first_pos++;
                second_pos++;
            }  
        }
    }
}
