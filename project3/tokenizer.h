#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>
# define MAXMENUARGS 7



struct job_info      //maybe move this to a different file.
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

int cmd_check(char *args[], struct job_info *queue[])
{
	
 if (strcmp(args[0], "help") == 0){
            print_help_menu();}

        else if (strcmp(args[0], "h") == 0){
            print_help_menu();}

        else if (strcmp(args[0], "?") == 0){
            print_help_menu();}
    
        else if(strcmp(args[0], "list") == 0){
            printf("list\n");
		    printf("\nhere is the submitted job: %s\n", queue[0]->name); // works but gives error if no jobs
		}


        else if(strcmp(args[0], "fcfs") == 0){
            printf("fcfs\n");}

        else if(strcmp(args[0], "sjf") == 0){
            printf("sjf\n");}

        else if(strcmp(args[0], "priority") == 0){ //This works Bitch
            printf("****priority****\n");}

        else if(strcmp(args[0], "test") == 0){
            printf("test\n");
        }

        else if((strcmp(args[0], "quit") == 0)){
            printf("quit\n");
            return 1;  // sets flag to signal end of use.
        }
        else
        {
            printf("command Not recognized\n");
        }
return 0;
}

