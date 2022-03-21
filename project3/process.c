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
#include <stdlib.h>
int main(int argc, char *argv[] )
{
 

 // we are converting from char[] to float
 // then multiplying the float by 1 mil to convert the 
 // time from seconds to micoseconds
 // then converts it to an int for the usleep
 // this all has to happen because sleep and usleep cannot take 
 // fractions of a second like 2.1 
 // so we have to convert it to the equivalent nanoseconds
 // There is probably a much better way to do this. But this works

  float dec_sleep_time;
  //printf("arg %s", argv[0]);
  
  dec_sleep_time = atof(argv[0]);

  dec_sleep_time = dec_sleep_time*1000000;

  int sleep_time = dec_sleep_time;
  //printf("sleep time %f ", sleep_time);

  usleep(sleep_time);
  //printf("done");
  return 0;
}

