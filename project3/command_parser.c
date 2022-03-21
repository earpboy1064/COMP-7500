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
//#include "menues.h"
#include "global.h"

#define CMD_BUF_SIZE 10 /* The size of the command queue */
#define NUM_OF_CMD   8  /* The number of submitted jobs   */
#define MAX_CMD_LEN  512 /* The longest commandline length */

# define MAXMENUARGS 8

void print_menu()
{
printf("\nWelcome to Wyatts AUbatch batch job scheduler Version 1.0 Type ‘help’ to find more about AUbatch commands.\n ");
};

void print_help_menu() // simple print menu can be changed later
{
printf("\nrun: <job> <time> <pri>: submit a job named <job>, execution time is <time>, priority is <pri>.\n");
printf("list: display the job status.\n");
printf("fcfs: change the scheduling policy to FCFS.\n");
printf("sjf: change the scheduling policy to SJF.\n");
printf("priority: change the scheduling policy to priority.\n");
printf("test <benchmark> <policy> <num_of_jobs> <priority_levels>\n<min_CPU_time> <max_CPU_time>\n");
printf("quit: Exit AUbatch\n");
}



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


void commandline_parser(char *input_cmd)
{
    //char *temp_cmd;
    
    size_t command_size;
    char *args[5];

   
    long mtime, secs, usecs;    


        int size = strlen(input_cmd);

        // removes the '\n' from getline input 
        // stackoverflow suggestion: shorturl.at/bhDEG
        if (input_cmd[size - 1] == '\n') input_cmd[--size] = 0;
 

       
        int total_arguments = 0;

        // breaks command up and puts the parts in args
        // sends temp command to be processed
        total_arguments = cmd_dispatch(input_cmd,args);

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
        
        queue[buf_head] = job_input;
        buf_head++;
        if (buf_head == CMD_BUF_SIZE)
            buf_head = 0;

        count++;
        
    /* ####################################################### */
    }



};

