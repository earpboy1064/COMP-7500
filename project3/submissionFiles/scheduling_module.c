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
#include "scheduling_module.h"
#include "global.h"


/*
This module handles scheduling thread and calling of the commandline parser

It controls the locking and unlocking of the queue using mutex and CVs







*/
void scheduling_module() // we need to accept jobs from commandline parser
{
    char *temp_cmd;
    size_t command_size;
    struct job_info job_input;
    
    /* Enter multiple commands in the queue to be scheduled */
    while (terminate == 0){
        /* lock the shared command queue */
        pthread_mutex_lock(&cmd_queue_lock);
        while (count == CMD_BUF_SIZE) {
            pthread_cond_wait(&queue_not_full, &cmd_queue_lock);
        }
        /* Dynamically create a buffer slot to hold a commandline */

        pthread_mutex_unlock(&cmd_queue_lock); // UNLOCK
        printf("Please submit a batch processing job:\n");
        printf(">"); 

        

        // collects command
        temp_cmd = malloc(MAX_CMD_LEN*sizeof(char));
        getline(&temp_cmd, &command_size, stdin);  
        int size = strlen(temp_cmd);

        // critical section 
        pthread_mutex_lock(&cmd_queue_lock);  

       
     
        commandline_parser(temp_cmd);

    

        if(count >= 1){
            pthread_cond_signal(&queue_not_empty); 
        }

        // allows for dispatch thread to shutdown
        if (terminate == 1 ){pthread_cond_signal(&queue_not_empty); }


        /* Unlok the shared command queue */
        pthread_mutex_unlock(&cmd_queue_lock);


    }
  

}
