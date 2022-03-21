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
#include "command_parser.h"

#include "global.h"

#define CMD_BUF_SIZE 10 /* The size of the command queue */
#define NUM_OF_CMD   8  /* The number of submitted jobs   */
#define MAX_CMD_LEN  512 /* The longest commandline length */

# define MAXMENUARGS 8

int cmd_dispatch(char *cmd, char* temp[]) {
	time_t beforesecs, aftersecs, secs;
	u_int32_t beforensecs, afternsecs, nsecs;
	char *args[MAXMENUARGS];
	int nargs=0;
	char *word;
	char *context;
 	int i, result;

	for (word = strtok_r(cmd, " ", &context);
	     word != NULL;
	     word = strtok_r(NULL, " ", &context)) {

		if (nargs >= MAXMENUARGS) {
			printf("Command line has too many words\n");
			
		}
		args[nargs] = word;
		temp[nargs] = word;
		nargs++;
	}

	if (nargs==0) {
		return 0;
	}
	
	return (nargs);
}


int cmd_check(char *args[], int total_args, int size_of_queue)
{
	
 if (strcmp(args[0], "help") == 0){
            print_help_menu();}

        else if (strcmp(args[0], "h") == 0){
            print_help_menu();}

        else if (strcmp(args[0], "?") == 0){
            print_help_menu();}
    
        else if(strcmp(args[0], "list") == 0){
            display_job_queue();
        }

        else if(strcmp(args[0], "fcfs") == 0){
            printf("Scheduling policy is switched to FCFS ");
            printf("All the %d waiting jobs \nhave been rescheduled.\n", count );
            policy = FCFS;
        }

        else if(strcmp(args[0], "sjf") == 0){
            printf("Scheduling policy is switched to SJF ");
            printf("All the %d waiting jobs \nhave been rescheduled.\n", count );
            policy = SJF; 
        }

        else if(strcmp(args[0], "priority") == 0){ //This works Bitch
            printf("Scheduling policy is switched to PRIORITY ");
            printf("All the %d waiting jobs \nhave been rescheduled.\n", count );
            policy = PRIORITY; 
        }       


        else if(strcmp(args[0], "test") == 0){
            
            // checks to makesure the correct number of arguments are passed.
           if(strcmp(args[1], "load") == 0 )
            {
             automated_performance_evaluation(args);
            }
           
            else if (total_args != 8) 
            {
                printf("please enter the correct number of arguments\n");
                printf("test <benchmark> <policy> <num_of_jobs> <arrival_rate> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
            }

            else
            {
            automated_performance_evaluation(args);
            }
            
        }

        else if((strcmp(args[0], "quit") == 0)){
            printf("quit\n");
            performance_measurements();
            //print totals
            return 1;  // sets flag to signal end of use.
        }

        else if((strcmp(args[0], "r") == 0)){}

        else
        {
            printf("command Not recognized\n");
        }
return 0;
}


void commandline_parser()
{
    char *temp_cmd;
    u_int i;
    size_t command_size;
    char *args[5];

   
    long mtime, secs, usecs;    

    /* Enter multiple commands in the queue to be scheduled */
    while (terminate == 0){
        /* lock the shared command queue */
        pthread_mutex_lock(&cmd_queue_lock);
        while (count == CMD_BUF_SIZE) {
            pthread_cond_wait(&queue_not_full, &cmd_queue_lock);
        }
        /* Dynamically create a buffer slot to hold a commandline */

        pthread_mutex_unlock(&cmd_queue_lock); // UNLOCK
   //     printf("\n********COMMAND_P UNLOCKED 2********\n");
        printf("Please submit a batch processing job:\n");
        printf(">"); 

        
        // collects command
        temp_cmd = malloc(MAX_CMD_LEN*sizeof(char));
        getline(&temp_cmd, &command_size, stdin);  
        int size = strlen(temp_cmd);

        pthread_mutex_lock(&cmd_queue_lock);  



        // removes the '\n' from getline input 
        // stackoverflow suggestion: shorturl.at/bhDEG
        if (temp_cmd[size - 1] == '\n') temp_cmd[--size] = 0;
 

       
        int total_arguments = 0;

        // breaks command up and puts the parts in args
        // sends temp command to be processed
        total_arguments = cmd_dispatch(temp_cmd,args);

        terminate = cmd_check(args, total_arguments, count);
        /*   Needs to collect new input after displaying the help   */

        // holds the job to be put into the queue.
        struct job_info job_input;
        
        // structure of command <command> <job name> <time?> <priority?> 

/* ######################## JOB SUBMISSION ############################### */


        // Switch to build job based on input parameters

        
        int i = 0;

        if (strcmp(args[0], "r") == 0){
            while (i < total_arguments) 
            {
        
             switch( i )
             {
                case 1:
                    snprintf(job_input.name, sizeof(job_input.name), "%s", args[1]);
                    //shorturl.at/mxzQ6  Stack overflow reference
                     break;
                case 2:
                     job_input.est_cpu_time = atof(args[3]);
                    break;
                case 3:
                    job_input.priority = atoi(args[2]);
             }
                gettimeofday(&job_input.wait_start, NULL);

            
             i++;
            }
    
        scheduing_module(job_input); // adds the job.
        count++;
        
    /* ####################################################### */
    }
    if(count >= 1){
        pthread_cond_signal(&queue_not_empty); 
    }

        // allows for dispatch thread to shutdown
        if (terminate == 1 ){pthread_cond_signal(&queue_not_empty); }


        /* Unlok the shared command queue */
        pthread_mutex_unlock(&cmd_queue_lock);
    }
};

