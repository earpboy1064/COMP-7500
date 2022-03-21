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
//#include "tokenizer.h"
#include "menues.h"
# define MAXMENUARGS 8

//############  From Aubatch_simple.c
typedef unsigned int u_int; 
//#define LOW_ARRIVAL_RATE /* Long arrivel-time interval */
#define LOW_SERVICE_RATE   /* Long service time */
/* 
 * Static commands are submitted to the job queue.
 * When comment out the following macro, job are submitted by users.
 */
//#define STATIC_COMMAND 
#define CMD_BUF_SIZE 10 /* The size of the command queue */
#define NUM_OF_CMD   8  /* The number of submitted jobs   */
#define MAX_CMD_LEN  512 /* The longest commandline length */


#define FCFS  1
#define SJF  2
#define PRIORITY  3



/* 
 * When a job is submitted, the job must be compiled before it
 * is running by the executor thread (see also executor()).
 */
void *commandline( void *ptr ); /* To simulate job submissions and scheduling */
void *executor( void *ptr );    /* To simulate job execution */
pthread_mutex_t cmd_queue_lock;  /* Lock for critical sections */
pthread_cond_t queue_not_full; /* Condition variable for buf_not_full */
pthread_cond_t queue_not_empty; /* Condition variable for buf_not_empty */
/* Global shared variables */
u_int buf_head;
u_int buf_tail;
u_int count;
u_int policy = SJF;
int terminate = 0; //-FLAG


//################

/* COMMENT BLOCK 
# Refrences:
# aubatch_sample.c Written by Xiao Qin 
#
#
#
# How to compile: 
# gcc -g -w AUbatch.c -o AUbatch -lpthread
#
#
#
#
#
*/

/*########## Data structures Start ##########*/
//d1



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
int completed_counter = 0;



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
			return E2BIG;
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




/*########## start of functions ##########*/
//1
//void scheduing_module(struct scheduling_policy policy, struct workload_info workload, struct job_info_queue job_info);

void commandline_parser( );

//2
void dispatching_module(); // job execution

//3
void scheduing_module(struct job_info job_input); // scheduling and submission

//4 
void display_job_queue();

//5
void help();

//6
void performance_measurements();

//7 
void automated_performance_evaluation();


void bubble_sort();
/*########## End of functions ##########*/


struct performance_info batch_totals;


int main( int argc, char *argv[])
{
    print_menu();
/* FROM aubtach_sample.c Written by Xiao Qin*/ 
    pthread_t command_thread, executor_thread; /* Two concurrent threads */
    char *message1 = "Command Thread";
    char *message2 = "Executor Thread";
    int  iret1, iret2;
    /* Initilize count, two buffer pionters */
    count = 0; 
    buf_head = 0;  
    buf_tail = 0; 
    /* Create two independent threads:command and executors */
   // iret1 = pthread_create(&command_thread, NULL, commandline, (void*) message1);
   // iret2 = pthread_create(&executor_thread, NULL, executor, (void*) message2);
    printf("\n\nwe compiled correctly!!\n\n");
    //iret1 = pthread_create(&command_thread, NULL, scheduing_module, (void*) message1);
    iret1 = pthread_create(&command_thread, NULL, commandline_parser,  NULL);
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
/*End of aubatch_sample*/


printf("\n\nThank you for using AUbatch\n\n");
return 0;
}




/*########## start of functions ##########*/
//1

void bubble_sort()
{
        
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

                else if(policy == PRIORITY) // job assignments swapped to sort in reverse order
                {
                    job_one = queue[first_pos].priority;
                    job_two =  queue[second_pos].priority;
                }

                 else if(policy == FCFS) // job assignments swapped to sort in reverse order
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





void scheduing_module(struct job_info job_input) // we need to accept jobs from commandline parser
{
    char *temp_cmd;
    u_int i;
    size_t command_size;
    char *args[5];



     
        queue[buf_head] = job_input;
        buf_head++;
        if (buf_head == CMD_BUF_SIZE)
            buf_head = 0;
        //bubble_sort();

        //printf("In commandline: queue[%d] = %s\n", count, queue[count]); 
  

}








void handler(sig) { // not used currently 

        // messed up needs to do this when its called 
   // long mtime, secs, usecs;    

        
    //gettimeofday(&queue[buf_tail].wait_end, NULL);
   // secs  = queue[buf_tail].wait_end.tv_sec  - queue[buf_tail].wait_start.tv_sec;
   // usecs = queue[buf_tail].wait_end.tv_usec - queue[buf_tail].wait_start.tv_usec;
   // mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;

   // printf("milli waited = %ld", mtime);

   // batch_totals.total_waiting_time += mtime;
}





// $$$$$$$$$$$$$$$$$$$$$ DISPATCH $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//2
void dispatching_module()
{
    
    while(terminate == 0){

        //######## queue handling and termination check ########//

        /* lock and unlock for the shared process queue */
        pthread_mutex_lock(&cmd_queue_lock);

        while ((count == 0) && (terminate ==0)) {
            pthread_cond_wait(&queue_not_empty, &cmd_queue_lock);
        }

        if (terminate ==1)// checks if terminate changed while in pthread_cond wait
        break;
        //######## queue handling and termination check ########//



        bubble_sort();



        signal(SIGCHLD,handler); // handler to catch sigchld to tell when execv is finished.
    
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
        //https://stackoverflow.com/questions/42840950/waitpid-and-fork-execs-non-blocking-advantage-over-a-syscall
        //https://stackoverflow.com/questions/43487950/how-to-handle-signals-when-process-is-waiting-in-waitpid
        

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




//4 
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

		    
                
            printf("%s %d    ",queue[temp_tail].name, i+1);
            printf("%.1f     ",queue[temp_tail].est_cpu_time);
            printf("%d      ",queue[temp_tail].priority);  
            printf("%.1f    ", queue[temp_tail].arrival_time);   
            printf("         %s\n", queue[temp_tail].progress );       
            temp_tail++; // works but gives error if no jobs
    }
}
/*
//5
void help(){}

//6
*/


void performance_measurements()
{


int num_jobs = batch_totals.num_jobs;

double total_waiting_time = batch_totals.total_waiting_time / 1000;

double total_cpu_time = batch_totals.total_cpu_time / 1000;

double total_time = total_cpu_time + total_waiting_time;

double Average_cpu_time = total_cpu_time /  num_jobs;


double Average_turn_around_time = (total_cpu_time + total_waiting_time) / num_jobs;


double Average_waiting_time = batch_totals.total_waiting_time / num_jobs;


double throughput = num_jobs / total_time;


    printf("\n\nTotal number of job submitted: %d\n",num_jobs);

    printf("Average turnaround time: %.2f seconds\n", Average_turn_around_time);

    printf("Average CPU time:        %.2f seconds\n",Average_cpu_time);

    printf("Average wait time:       %.2f seconds\n",Average_waiting_time / 1000);

    printf("throughput:              %.3f No./second\n",throughput );

        //printf("Average turnaround time: %d\n", batch_totals.num_jobs);


}

//7 


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
  //char policy[]

  if(strcmp(args[1], "load") ==0 )
   {
     struct job_info test_batch[5];

     snprintf(test_batch[0].name, sizeof(test_batch[0].name), "%s", "process");
     snprintf(test_batch[1].name, sizeof(test_batch[1].name), "%s", "process");
     snprintf(test_batch[2].name, sizeof(test_batch[2].name), "%s", "process");
     snprintf(test_batch[3].name, sizeof(test_batch[3].name), "%s", "process");
     snprintf(test_batch[4].name, sizeof(test_batch[4].name), "%s", "process");

     test_batch[0].arrival_time = 0; // prob should be actual time
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



     printf("start time: %ld \n", test_batch[0].wait_start.tv_sec);
     printf("start time:  %ld \n", test_batch[1].wait_start.tv_sec);
     printf("start time:  %ld \n", test_batch[2].wait_start.tv_sec);
     printf("start time:  %ld \n", test_batch[3].wait_start.tv_sec);


     int i = 0;
   

     for( i = 0; i < 5; i++)
        {

            queue[buf_head] = test_batch[i];
            printf("\nhere is job: %d - %s\n",count, queue[buf_head].name); // works but gives error if no jobs

            buf_head++;
            count++;
            if (buf_head == CMD_BUF_SIZE)
            buf_head = 0;

            //printf("bufhead: %d", buf_head);

        }

    }

    else 
    {
        int error = 0; // tells the rest of benchmark not to run


        if (strcmp(args[1], "benchmark") == 1){error = 1;}
        else if (strcmp(args[2], "BENCHMARK") == 1){error = 1;}
        else if (strcmp(args[2], "bench") == 1){error = 1;}
        else if (strcmp(args[2], "BENCH") == 1){error = 1;}
        
       
        // works for all capitalized or lower case policy.
        if (strcmp(args[2], "fcfs") == 0){ policy = FCFS; }
        else if (strcmp(args[2], "FCFS") == 0){ policy = FCFS;}

        else if (strcmp(args[2], "priority") == 0){policy = PRIORITY;}
        else if (strcmp(args[2], "PRIORITY") == 0){policy = PRIORITY;}

        else if(strcmp(args[2], "sjf") == 0){ policy = SJF; }
        else if(strcmp(args[2], "SJF") == 0){ policy = SJF; }

        else 
        {
            printf("ERROR! \nPlease check your policy choice\n <fcfs> <priority> <sjf>\n");
            error = 1;
        }


        if(error == 0 )
        {
          num_jobs = atoi(args[3]);
          arrival_rate = atof(args[4]);
          pri_levels = atoi(args[5]);
          sscanf(args[6], "%lf", &min_cpu); 
          sscanf(args[7], "%lf", &max_cpu);
        
        
          double random_cpu_time, random_arrival_rate;

          srand ( time ( NULL));


          //test bench priority 5 3 1 5

          int i = 0;
          for( i =0; i < num_jobs; i++)
          {
                
             // reference https://stackoverflow.com/questions/33058848/generate-a-random-double-between-1-and-1

             double range = (max_cpu - min_cpu); 

             double div = RAND_MAX / range;

             random_cpu_time = min_cpu + (rand() / div); // random cpu time

             queue[buf_head].est_cpu_time = random_cpu_time; // cpu time



             
             div = RAND_MAX / arrival_rate;
             random_arrival_rate = (rand() / div);
             queue[buf_head].arrival_time = random_arrival_rate; // cpu time



             queue[buf_head].priority = rand() % pri_levels+1; // priority

             snprintf(queue[buf_head].name, sizeof(queue[buf_head].name), "%s", "process"); //name

             gettimeofday(&queue[buf_head].wait_start, NULL); // waiting time

             strcpy(queue[buf_head].progress,""); // progress

             buf_head++;
             count++;
             if (buf_head == CMD_BUF_SIZE)
                buf_head = 0;

            }

        }

        else 
        {
         printf("\n**please enter the correct arguments**");
         printf("\ntest <benchmark> <policy> <num_of_jobs> <arrival_rate>");
         printf(" <priority_levels <min_CPU_time> <max_CPU_time> \n\n");

        }
    }
    
    //bubble_sort();

}
