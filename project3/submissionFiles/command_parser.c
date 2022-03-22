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

#define CMD_BUF_SIZE 50 /* The size of the command queue */
#define NUM_OF_CMD   8  /* The number of submitted jobs   */
#define MAX_CMD_LEN  512 /* The longest commandline length */

# define MAXMENUARGS 8


/*
    Command Parser   

    this module process commands and calls the appropriate functions
    it is responsable for parsing the command as well as checking it for meaning   
    as this function is called in the sheduler it opperates in a critical section 
    meaning this is allowed to view and modify the queue.


    refrences:
    //shorturl.at/mxzQ6  SO reference
    //shorturl.at/bhDEG  SO reference

*/










// menu functions
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


// tokenizer for command
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



// checks command for meaning
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


        // following three change policy if requested
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

        else if(strcmp(args[0], "priority") == 0){
            printf("Scheduling policy is switched to PRIORITY ");
            printf("All the %d waiting jobs \nhave been rescheduled.\n", count );
            policy = PRIORITY; 
        }       


        // runs the approriate test function
        else if(strcmp(args[0], "test") == 0){
            
            // checks to makesure the correct number of arguments are passed.
           if((total_args == 2) && (strcmp(args[1], "load") == 0) )
            {
             automated_performance_evaluation(args);
            }
           
            else if (total_args < 8) 
            {
                printf("please enter the correct number of arguments\n");
                printf("test <benchmark> <policy> <num_of_jobs> <arrival_rate> <priority_levels> <min_CPU_time> <max_CPU_time>\n");
            }

            else
            {
            automated_performance_evaluation(args);
            }
            
        }

        // returns terminate signal for threads
        else if((strcmp(args[0], "quit") == 0)){
            printf("quit\n");
            performance_measurements();
            return 1;  // sets flag to signal end of use.
        }

        else if((strcmp(args[0], "r") == 0)){}

        else if((strcmp(args[0], "run") == 0)){}


        else
        {
            printf("command Not recognized\n");
        }
return 0;
}


// prints the job queue and relevent information
void display_job_queue()
{
    int i =0; 
            
    int temp_tail = buf_tail;

    printf("\nTotal number of jobs in the queue: %d",count);
            printf("\nScheduling Policy: ");
            switch(policy)
            {
                case (1):
                    printf("FCFS.\n");
                    break;
                case(2):
                    printf("SJF.\n");
                    break;
                case(3):
                    printf("PRIORITY\n");
                    break;
            } 


    printf("Name     CPU_Time    Pri    Arrival_time    Progress \n");
    for( i = 0; i < count; i++)
    {
        if (temp_tail == CMD_BUF_SIZE){ temp_tail = 0; }

		    
                
            printf("%s %d    ",queue[temp_tail].name,  queue[temp_tail].process_num);
            printf("%.1f     ",queue[temp_tail].est_cpu_time);
            printf("%d      ",queue[temp_tail].priority);  
            printf("%.1f    ", queue[temp_tail].arrival_time);   
            printf("         %s\n", queue[temp_tail].progress );       
            temp_tail++; // works but gives error if no jobs
    }
}




// process the the commands sent to the scheduling module
void commandline_parser(char *input_cmd)
{
    
    size_t command_size;
    char *args[5];

   
    long mtime, secs, usecs;    


        int size = strlen(input_cmd);

        // removes the '\n' from getline input 
        if (input_cmd[size - 1] == '\n') input_cmd[--size] = 0;
 

       
        int total_arguments = 0;

        // breaks command up and puts the parts in args
        // sends temp command to be processed
        total_arguments = cmd_dispatch(input_cmd,args);

        terminate = cmd_check(args, total_arguments, count);
        /*   Needs to collect new input after displaying the help   */

        // holds the job to be put into the queue.
        struct job_info job_input;
        
        // structure of command <command> <job name> <time> <priority?> 

/* ######################## JOB SUBMISSION ############################### */


        // Switch to build job based on input parameters
        int i = 0;
        int run_true; // flag to signal wrong name
        if (strcmp(args[0], "r") == 0) { run_true = 1;}
        else if (strcmp(args[0], "run") == 0) { run_true = 1;}

        // builds the job
        if ( run_true == 1 )
        {
            if(total_arguments > 1)
            {
                while (i < total_arguments) 
                {
               
                switch( i )
                {
                    case 1:
                        snprintf(job_input.name, sizeof(job_input.name), "%s", args[1]);
                         break;
                    case 2:
                        job_input.est_cpu_time = atof(args[2]);
                        break;
                    case 3:
                        job_input.priority = atoi(args[3]);
                }
                    gettimeofday(&job_input.wait_start, NULL);
                    strcpy(queue[buf_head].progress,""); // progress
                    queue[buf_head].process_num = 0;

                i++;
                }


                
                queue[buf_head] = job_input;
                buf_head++;
                if (buf_head == CMD_BUF_SIZE)
                    buf_head = 0;

                count++;



                printf("Job %s was submitted.\n",args[1]);
                printf("Total number of jobs in the queue: %d\n",count);
                printf("Scheduling Policy: ");
            switch(policy)
            {
                case (1):
                    printf("FCFS.\n");
                    break;
                case(2):
                    printf("SJF.\n");
                    break;
                case(3):
                    printf("PRIORITY\n");
                    break;
            } 
            


            }
    /* ####################################################### */
        }

};

