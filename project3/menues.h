#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>

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