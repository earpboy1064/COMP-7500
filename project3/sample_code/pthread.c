#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>



void* function_one()
{
    
char *my_args[5];
my_args[0] = "process";
my_args[1] = "-help";
my_args[2] = "-setup";
my_args[3] = NULL;
/* This is processed by the child */
      execv("process", my_args);
      puts("Uh oh! If this prints, execv() must have failed");
};


void* function_two()
{
printf("inside thread two\n");
};



int main(int argc, char *argv[])
{

pthread_t tid_one, tid_two;

pthread_create(&tid_one, NULL, &function_one, NULL);


pthread_create(&tid_two, NULL, &function_two, NULL);


sleep(5);
return 0;
}