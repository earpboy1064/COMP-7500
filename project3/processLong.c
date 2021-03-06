/*
 * gcc process.c -o process
 * 
 * Reference: 
 * https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?
answer=1044654269&id=1043284392
 * Modified by Xiao Qin
 * Department of Computer Science and Software Engineering
 * Auburn University
 * Feb. 6, 2018.
 */
#include <stdio.h> 
#include <unistd.h>
int main(int argc, char *argv[] )
{
  int i = 1;
  //printf("A process is running ...\n");
  argc--;
  /* Print all the arguments passed from the commandline */
  while (argc > 0) {
    sleep(5);
    printf ("Argument %d is: %s\n", i, argv[i]);
    i++;
    argc--;
    
  }

  sleep(100);
  printf("process Done\n");
   return 0;
}


