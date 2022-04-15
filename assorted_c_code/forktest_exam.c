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

int count = 3;

void test()
{
printf("hello ");

}

int main(int argc, char *argv[]) {
	pid_t pid;
	printf("yo ");
	pid = fork();
	if (pid == 0) {
		fork();
		pthread_t command_thread, executor_thread, another_thread;
		int iret2 = pthread_create(&command_thread, NULL, test,  NULL);
		int iret3 = pthread_create(&executor_thread, NULL, test,  NULL);
		int iret4 = pthread_create(&another_thread, NULL, test,  NULL);
		pthread_join(command_thread, NULL);
    	pthread_join(executor_thread, NULL); 
		pthread_join(another_thread, NULL); 

	}
	fork();
	fork();
	return 0;
}
