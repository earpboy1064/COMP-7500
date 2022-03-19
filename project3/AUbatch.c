#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
//#include "tokenizer.h"
#include "menues.h"
# define MAXMENUARGS 7

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

//struct job_info *queue[];
















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
int terminate = 0; //-FLAG












# define MAXMENUARGS 7



struct job_info      //maybe move this to a different file.
{
    double exec_time;  // this might be the time to run
    double est_cpu_time;
    int priority;
    double start_time;
    double finish_time;
    int wait;
    double arrival_time;
    char* name[100];
    char progress[5];

    // can add arrival_time if needed
};


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

int cmd_check(char *args[] /*, struct job_info *queue[]*/, int size_of_queue)
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
            printf("fcfs\n");}

        else if(strcmp(args[0], "sjf") == 0){
            printf("sjf\n");}

        else if(strcmp(args[0], "priority") == 0){ //This works Bitch
            printf("****priority****\n");}

        else if(strcmp(args[0], "test") == 0){
            //temp
            automated_performance_evaluation(queue);
            int i =0;
            for( i = 0; i < 5; i++)
   
            printf("test\n");
        }

        else if((strcmp(args[0], "quit") == 0)){
            printf("quit\n");
            return 1;  // sets flag to signal end of use.
        }

        else if((strcmp(args[0], "r") == 0)){}

        else
        {
            printf("command Not recognized\n");
        }
return 0;
}



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
//void scheduing_module(struct scheduling_policy policy, struct workload_info workload, struct job_info_queue job_info);

void commandline_parser();

//2
void *dispatching_module(void *ptr); // job execution

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








//struct job_info *queue[];   actual queue to be used



//snprintf(job_one.name, sizeof(job_one.name), "%s", "process");

//job_one.name() = "process";

//struct job_info queue[50];




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
    iret1 = pthread_create(&command_thread, NULL, commandline_parser,NULL);
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


printf("\n\nThank you for using AUbatch\n\n");
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
                    job_one = queue[j].est_cpu_time;
                    job_two =  queue[j+1].est_cpu_time;  
                }

                else if(policy == PRIORITY) // job assignments swapped to sort in reverse order
                {
                    job_two = queue[j].priority;
                    job_one =  queue[j+1].priority;
                }

                if(job_one > job_two)
                {
                    //swap elements
                    struct job_info *temp;
                    temp = &queue[j];
                    queue[j] = queue[j+1];
                    queue[j+1] = *temp;
                }
            }  
         }
    


}








/*
void scheduing_module(struct scheduling_policy policy, struct workload_info workload, struct job_info job)
{

}
*/

void commandline_parser()
{
    char *temp_cmd;
    u_int i;
    char num_str[8];
    size_t command_size;
    char *args[5];


    /* Enter multiple commands in the queue to be scheduled */
    //for (i = 0; i < NUM_OF_CMD; i++) { 
    while (terminate == 0){
        /* lock the shared command queue */
        pthread_mutex_lock(&cmd_queue_lock);
     //   printf("\n********COMMAND_P locked 1********\n");
        //printf("In commandline: count = %d\n", count);
       // printf("In commandline_parser\n");
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
//        printf("\n********COMMAND_P locked 3********\n");


/* ####################################################### */

        // removes the '\n' from getline input 
        // stackoverflow suggestion: shorturl.at/bhDEG
        if (temp_cmd[size - 1] == '\n') temp_cmd[--size] = 0;
 

       
        int total_arguments = 0;
        


        // breaks command up and puts the parts in args
        // sends temp command to be processed
        total_arguments = cmd_dispatch(temp_cmd,args);

        terminate = cmd_check(args, /*queue,*/ count);
        /*   Needs to collect new input after displaying the help   */


        //printf("total_arguments: %d", total_arguments);

        //printf("\ntemp_cmd is %s\n", temp_cmd);


        // holds the job to be put into the queue.
        struct job_info job_input;
        


        // structure of command <command> <job name> <time?> <priority?> 

        // Switch to build job based on input parameters
        int i = 0;

    if (strcmp(args[0], "r") == 0){
        while (i < total_arguments) 
        {
        
            switch( i ){
                case 1:
                    snprintf(job_input.name, sizeof(job_input.name), "%s", args[1]);
                    //shorturl.at/mxzQ6  Stack overflow reference
                     break;
                case 2:
                    job_input.priority = atoi(args[2]);
                    break;
            }
            i++;
        }
    
        scheduing_module(job_input);

       // queue[count] = &job_input; 
        count++;
        //bubble_sort();
        //printf("In commandline: queue[%d] = %s\n", count, queue[count]); 
    

    /* ####################################################### */


        //pthread_cond_signal(&queue_not_empty);  
    }
            //bubble_sort();
    if(count >= 1){
        pthread_cond_signal(&queue_not_empty); 
    }

        if (terminate == 1 ){        pthread_cond_signal(&queue_not_empty); }
        /* Unlok the shared command queue */
        /* Simulate a low arrival rate */
        pthread_mutex_unlock(&cmd_queue_lock);
     //   printf("\n********COMMAND_P UNLOCKED 4********\n");



    }



};





// Currently being called my thread !!!!!!
void scheduing_module(struct job_info job_input) // we need to accept jobs from commandline parser
{
    char *message;
    char *temp_cmd;
    u_int i;
    char num_str[8];
    size_t command_size;
    char *args[5];



     
        //queue[count] = &job_input;  old queuestyle prob best though
        queue[buf_head] = job_input;
        //count++;
        buf_head++;
        if (buf_head == CMD_BUF_SIZE)
            buf_head = 0;
        //bubble_sort();

        //printf("In commandline: queue[%d] = %s\n", count, queue[count]); 
  

}








void handler(sig) {
completed_job_queue[completed_counter] = queue[buf_tail];
completed_counter++;
}


// $$$$$$$$$$$$$$$$$$$$$ DISPATCH $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//2
void *dispatching_module(void *ptr)
{
char *message;
    u_int i;
    message = (char *) ptr;
    //printf("%s \n", message);
   // for (i = 0; i < NUM_OF_CMD; i++) {
    while(terminate == 0){
        /* lock and unlock for the shared process queue */
        pthread_mutex_lock(&cmd_queue_lock);
     //   printf("\n********DISPATCH_MOD LOCKED********\n");

    //    printf("In dispatch: count = %d\n", count);
        while ((count == 0) && (terminate ==0)) {
            //printf("\ndispatch waiting\n");
            pthread_cond_wait(&queue_not_empty, &cmd_queue_lock);
        }

        if (terminate ==1)// checks if terminate changed while in pthread_cond wait
        break;



      

        char temp[100];
        //strcpy(temp, queue[0]->name); // converting char* to char[] for execv
        strcpy(temp, queue[buf_tail].name);
        strcpy(queue[buf_tail].progress,"Run");
        pid_t pid; // holds pid of child
  
        signal(SIGCHLD,handler); // handler to catch sigchld to tell when execv is finished.

        



        // this converts from double to string to be passed to argv
        double temp_double = queue[buf_tail].est_cpu_time;
        char arg_convert[5];
        // only converts single decimal place
        sprintf(arg_convert,"%.1f",queue[buf_tail].est_cpu_time);


        char *my_args[5]; 
        my_args[0] = arg_convert;
        my_args[1] = NULL;

        //puts("fork()ing");
        switch ((pid = fork()))
        {
            case -1:
                /* Fork() has failed */
                perror("fork");
                break;
            case 0:
                /* This is processed by the child */
                
                execv(temp, my_args);               
                puts("Uh oh! If this prints, execv() must have failed");
                exit(EXIT_FAILURE);
                break;
            default:
                /* This is processed by the parent */
             //   puts ("This is a message from the parent");
                break;
        }

        pthread_cond_signal(&queue_not_full);
        /* Unlok the shared command queue */
        pthread_mutex_unlock(&cmd_queue_lock);
        int w, status;
        w = waitpid(pid, NULL, 0 );

        //printf("\nValue of w %d\n", w);


        //https://stackoverflow.com/questions/42840950/waitpid-and-fork-execs-non-blocking-advantage-over-a-syscall
        //https://stackoverflow.com/questions/43487950/how-to-handle-signals-when-process-is-waiting-in-waitpid
        
        //system(temp); // run job in first position
                      // works great but program has to wait defeates the purpose 

        

        count--; //remove when useing others.
        buf_tail++;
         if (buf_tail == CMD_BUF_SIZE)
            buf_tail = 0;

      //  printf("Job is running...\n");
        /* Free the dynamically allocated memory for the buffer */
        //free(queue[buf_tail]);
     
        /* Move buf_tail forward, this is a circular queue */ 
       // pthread_cond_signal(&queue_not_full);
        /* Unlok the shared command queue */
       // pthread_mutex_unlock(&cmd_queue_lock);
     //   printf("\n********DISPATCH_MOD UNLOCKED********\n");
       // w = waitpid(pid, &status, WNOHANG ); // this doesnt hang but only catches it after the next process starts.
    } /* end for */

    
}; 




//4 
void display_job_queue()
{
    int i =0; 
            
    int temp_tail = buf_tail;


   

    printf("\nTotal number of jobs in the queue: %d",count);
            printf("\nScheduling Policy:");
            switch(policy)
            {
                case (1):
                    printf("FCFS.\n");
                    break;
                case(2):
                    printf("SJF.\n");
                    break;
                case(3):
                    printf("PRIORIY\n");
                    break;
            } 


    printf("Name     CPU_Time    Pri    Arrival_time    Progress \n");
    for( i = 0; i < count; i++)
    {
        if (temp_tail == CMD_BUF_SIZE){ temp_tail = 0; }

		    
                
            printf("%s      ",queue[temp_tail].name);
            printf("%.1f     ",queue[temp_tail].est_cpu_time);
            printf("%d      ",queue[temp_tail].priority);  
            printf("%.1f    ", queue[temp_tail].arrival_time);   
            printf("    %s\n", queue[temp_tail].progress );       
            temp_tail++; // works but gives error if no jobs
        }
}
/*
//5
void help(){}

//6
void performance_measurements(){}

//7 

*/
/*########## End of functions ##########*/


void automated_performance_evaluation(/*struct job_info *queue[]*/)
{

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




    test_batch[0].est_cpu_time = 10;
    test_batch[1].est_cpu_time = 1;
    test_batch[2].est_cpu_time = 6;
    test_batch[3].est_cpu_time = 1;
    test_batch[4].est_cpu_time = 2;

    int i = 0;
   

    for( i = 0; i < 5; i++)
            {

            queue[buf_head] = test_batch[i];
            printf("\nhere is job: %d - %s\n",count, queue[buf_head].name); // works but gives error if no jobs

            buf_head++;
            count++;
            if (buf_head == CMD_BUF_SIZE)
            buf_head = 0;

            printf("bufhead: %d", buf_head);

            }


};
