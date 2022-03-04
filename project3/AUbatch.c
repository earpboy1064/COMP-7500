#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>


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

/*########## Data structures END ##########*/



/*########## start of functions ##########*/
//1
void scheduing_module(struct scheduling_policy policy, struct workload_info workload, struct job_info_queue job_info);

//2
void *dispatching_module(void *ptr); // job execution

//3
void *commandline_parser(void *ptr); // scheduling and submission

//4 
void display_job_queue();

//5
void help();

//6
void performance_measurements();

//7 
void automated_performance_evaluation();
/*########## End of functions ##########*/




// practice function can be removed
void *myThreadFun(void *vargp)
{
    sleep(1);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}

pthread_mutex_t queue_lock;
pthread_cond_t  buf_not_full;
pthread_cond_t buf_not_empty;

u_int buf_head;  // u_int used to prevent negative numbers
u_int buf_tail;
u_int count;

char *cmd_buffer[CMD_BUF_SIZE];



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
    iret1 = pthread_create(&command_thread, NULL, commandline, (void*) message1);
    iret2 = pthread_create(&executor_thread, NULL, executor, (void*) message2);
    /* Initialize the lock the two condition variables */
    pthread_mutex_init(&cmd_queue_lock, NULL);
    pthread_cond_init(&cmd_buf_not_full, NULL);
    pthread_cond_init(&cmd_buf_not_empty, NULL);
     
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

void *executor(void *ptr) // exampel of a dispatching module
{
    char *message;
    u_int i;
    message = (char *) ptr;
    printf("%s \n", message);
    for (i = 0; i < NUM_OF_CMD; i++) {
        /* lock and unlock for the shared process queue */
        pthread_mutex_lock(&cmd_queue_lock);
        printf("In executor: count = %d\n", count);
        while (count == 0) {
            pthread_cond_wait(&cmd_buf_not_empty, &cmd_queue_lock);
        }
        /* Run the command scheduled in the queue */
        count--;
        printf("In executor: cmd_buffer[%d] = %s\n", buf_tail, 
cmd_buffer[buf_tail]); 
        
        /* 
         * Note: system() function is a simple example.
         * You should use execv() rather than system() here.
         */
        system(cmd_buffer[buf_tail]); 
        /* Free the dynamically allocated memory for the buffer */
        free(cmd_buffer[buf_tail]);
#ifdef LOW_SERVICE_RATE
        sleep(2); /* Simulate service time of 2 seconds */
#endif
     
        /* Move buf_tail forward, this is a circular queue */ 
        buf_tail++;
        if (buf_tail == CMD_BUF_SIZE)
            buf_tail = 0;
        pthread_cond_signal(&cmd_buf_not_full);
        /* Unlok the shared command queue */
        pthread_mutex_unlock(&cmd_queue_lock);
    } /* end for */
}






//3
void commandline_parser(){};


void *commandline(void *ptr) // example of commandline_parser
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
 
        printf("In commandline: count = %d\n", count);
        while (count == CMD_BUF_SIZE) {
            pthread_cond_wait(&cmd_buf_not_full, &cmd_queue_lock);
        }
        /* Dynamically create a buffer slot to hold a commandline */
#ifdef STATIC_COMMAND
        cmd_buffer[buf_head] = malloc(strlen("process -help -time ") + 1);
        strcpy(cmd_buffer[buf_head], "./process -help -time "); 
        sprintf(num_str, "%d", i);
        strcat(cmd_buffer[buf_head], num_str);
#else
        pthread_mutex_unlock(&cmd_queue_lock);
        printf("Please submit a batch processing job:\n");
        printf(">"); 
        temp_cmd = malloc(MAX_CMD_LEN*sizeof(char));
        getline(&temp_cmd, &command_size, stdin);  
        pthread_mutex_lock(&cmd_queue_lock);    
        cmd_buffer[buf_head]= temp_cmd; 
        
#endif
        printf("In commandline: cmd_buffer[%d] = %s\n", buf_head, 
cmd_buffer[buf_head]);  
    
        count++;
 
        /* Move buf_head forward, this is a circular queue */ 
        buf_head++;
        if (buf_head == CMD_BUF_SIZE)
            buf_head = 0;
        pthread_cond_signal(&cmd_buf_not_empty);  
        /* Unlok the shared command queue */
        pthread_mutex_unlock(&cmd_queue_lock);
        /* Simulate a low arrival rate */
#ifdef LOW_ARRIVAL_RATE
        sleep(2); /* Simulate an arrival time of 2 seconds */
#endif
    } /* end for */
}





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
