#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


/*
Thread practice for assignment 3
simple refrence: 
    https://www.thegeekstuff.com/2012/04/create-threads-in-linux/

Really good youtube channel:
    https://www.youtube.com/watch?v=ln3el6PR__Q&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&index=6



Things to do: 
    create an array of strings where the standard producer consumer problem is solved using threads and CV 
    This will stand as the basis behind assignment 3 
*/



void* proccess_exec()
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

void* function_one(void *arg)
{
printf("inside thread one, adding values strings containing fruit\n");
//char *arr = (int * )queue;
char * queue = malloc(100); // this must be malloc because when the function ends it frees the memory and deletes our data since we are just passing a pointer
*queue = "Apple";
return (void *) queue;
}
/*
void* function_two(void* arg)
{
printf("inside thread two adding strings containing cars \n");
//char *arr = (char * )queue;

 *(char * )arg[4] = "F150";
 *(char * )arg[5] = "maxima";
 *(char * )arg[6] = "Dart";
 *(char * )arg[7] = "Demon";


}
*/


int main(int argc, char *argv[])
{

char * queue[100];

pthread_t tid_one, tid_two;

pthread_create(&tid_one, NULL, &function_one, (void*) queue);

if(pthread_join(tid_one, (void**) &queue) !=0)
{
    return 2;
}
free(queue);
//pthread_create(&tid_two, NULL, &function_two, (void*) queue);

sleep(5);
int i = 0; 
for( i = 0; i < 1; i ++ ) 
{
printf("%s\n", queue[i]);

}


return 0;
}