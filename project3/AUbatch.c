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

//############  From Aubatch_simple.c
typedef unsigned int u_int; 
//#define LOW_ARRIVAL_RATE /* Long arrivel-time interval */
#define LOW_SERVICE_RATE   /* Long service time */
/* 
 * Static commands are submitted to the job queue.
 * When comment out the following macro, job are submitted by users.
 */
//#define STATIC_COMMAND 
#define CMD_BUF_SIZE 100 /* The size of the command queueu */
#define NUM_OF_CMD   5  /* The number of submitted jobs   */
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
u_int policy = FCFS;




//################

/* COMMENT BLOCK 
# Refrences:
# aubatch_sample.c Written by Xiao Qin 
#
#
#
# How to compile: 
# gcc AUbatch.c -o Aubatch -lpthread
#
#
#
#
#
*/

/*########## Data structures Start ##########*/
//d1
struct job_info
{
    int argv;  // this might be the time to run
    double est_cpu_time;
    int priority;
    double start_time;
    double finish_time;
    int wait;
    char* name[100];

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

/*########## Data structures END ##########*/



/*########## start of functions ##########*/
//1
void scheduing_module(struct scheduling_policy policy, struct workload_info workload, struct job_info_queue job_info);

//2
void *dispatching_module(void *ptr); // job execution

//3
void *scheduing_module(void *ptr); // scheduling and submission

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






//char *queue[CMD_BUF_SIZE];
struct job_info *queue[];



int main( int argc, char *argv[])
{
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
    iret1 = pthread_create(&command_thread, NULL, scheduing_module, (void*) message1);
    iret2 = pthread_create(&executor_thread, NULL, dispatching_module, (void*) message2);
   
    /* Initialize the lock the two condition variables */
    pthread_mutex_init(&cmd_queue_lock, NULL);
    pthread_cond_init(&queue_not_full, NULL);
    pthread_cond_init(&queue_not_empty, NULL);
     
    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */
    pthread_join(command_thread, NULL);
    pthread_join(executor_thread, NULL); 
    printf("command_thread returns: %d\n",iret1);
    printf("executor_thread returns: %d\n",iret1);
/*End of aubatch_sample*/



return 0;
}




/*########## start of functions ##########*/
//1


void bubble_sort()
{

        
       // Sorting using bubble sort
        int i = 0;
        int j = 0;
        double job_one, job_two;
        for (i = 0; i < count - 1; i++)
        {
            for (j = 0; j < count-i-1; j++)
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








/*
void scheduing_module(struct scheduling_policy policy, struct workload_info workload, struct job_info job)
{

}
*/



// Currently being called my thread !!!!!!
void *scheduing_module(void *ptr) // we need to accept jobs from commandline parser
{
    char *message;
    char *temp_cmd;
    u_int i;
    char num_str[8];
    size_t command_size;
     
    message = (char *) ptr;
    printf("%s \n", message);
    /* Enter multiple commands in the queue to be scheduled */


    for (i = 0; i < NUM_OF_CMD; i++) { 

        /* lock the shared command queue */
        pthread_mutex_lock(&cmd_queue_lock);
        printf("\n********COMMAND_P locked 1********\n");
        //printf("In commandline: count = %d\n", count);
        printf("In commandline_parser\n");
        while (count == CMD_BUF_SIZE) {
            pthread_cond_wait(&queue_not_full, &cmd_queue_lock);
        }
        /* Dynamically create a buffer slot to hold a commandline */

        pthread_mutex_unlock(&cmd_queue_lock); // UNLOCK
        printf("\n********COMMAND_P UNLOCKED 2********\n");
        printf("Please submit a batch processing job:\n");
        printf(">"); 

        
        // collects command
        temp_cmd = malloc(MAX_CMD_LEN*sizeof(char));
        getline(&temp_cmd, &command_size, stdin);  
        int size = strlen(temp_cmd);

        // removes the '\n' from getline input 
        // stackoverflow suggestion: shorturl.at/bhDEG
        if (temp_cmd[size - 1] == '\n') temp_cmd[--size] = 0;
 
        pthread_mutex_lock(&cmd_queue_lock);  
        printf("\n********COMMAND_P locked 3********\n");
        printf("\ntemp_cmd is %s\n", temp_cmd);

        struct job_info job_input;

        snprintf(job_input.name, sizeof(job_input.name), "%s", temp_cmd);
        //shorturl.at/mxzQ6  Stack overflow reference

        printf("\nJob_input.name is: %s\n", job_input.name);
        queue[count] = &job_input; 
        printf("job added");
        count++;
        //bubble_sort();
        //printf("In commandline: queue[%d] = %s\n", count, queue[count]);  
    

        pthread_cond_signal(&queue_not_empty);  
        /* Unlok the shared command queue */
        /* Simulate a low arrival rate */
        pthread_mutex_unlock(&cmd_queue_lock);
        printf("\n********COMMAND_P UNLOCKED 4********\n");



    }

}




//2
void *dispatching_module(void *ptr)
{
char *message;
    u_int i;
    message = (char *) ptr;
    //printf("%s \n", message);
    for (i = 0; i < NUM_OF_CMD; i++) {
        /* lock and unlock for the shared process queue */
        pthread_mutex_lock(&cmd_queue_lock);
        printf("\n********DISPATCH_MOD LOCKED********\n");

        printf("In dispatch: count = %d\n", count);
        while (count == 0) {
            printf("\ndispatch waiting\n");
            pthread_cond_wait(&queue_not_empty, &cmd_queue_lock);
        }
        /* Run the command scheduled in the queue */
        //count--;
        //printf("In executor: queue[%d] = %s\n", buf_tail, queue[buf_tail]); 
        
        /* 
         * Note: system() function is a simple example.
         * You should use execv() rather than system() here.
         */

       
        printf("\nhere is the submitted job: %s\n", queue[0]->name);

       

        char temp[100];
        strcpy(temp, queue[0]->name); // converting char* to char[] for execv

        char *my_args[5];
        pid_t pid; // holds pid of child
  
        my_args[0] = "process";
        my_args[1] = "1";
        my_args[2] = "2";
        my_args[3] = NULL;
  
        puts("fork()ing");
  
        char  test[100] = "processLong";
        switch ((pid = fork()))
        {
            case -1:
                /* Fork() has failed */
                perror("fork");
                break;
            case 0:
                /* This is processed by the child */
                execv(temp, NULL);               
                puts("Uh oh! If this prints, execv() must have failed");
                exit(EXIT_FAILURE);
                break;
            default:
                /* This is processed by the parent */
                puts ("This is a message from the parent");
                break;
        }
  
        //system(temp); // run job in first position
                      // works great but program has to wait defeates the purpose 


        count--;
        printf("Job is running...\n");
        /* Free the dynamically allocated memory for the buffer */
        //free(queue[buf_tail]);
     
        /* Move buf_tail forward, this is a circular queue */ 
        pthread_cond_signal(&queue_not_full);
        /* Unlok the shared command queue */
        pthread_mutex_unlock(&cmd_queue_lock);
        printf("\n********DISPATCH_MOD UNLOCKED********\n");

    } /* end for */

    
}; 


/*

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

*/
/*########## End of functions ##########*/
