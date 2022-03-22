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
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <math.h>
#include <time.h>


/*
Program calculates primes.
it will calculate primes then preform time check to see if it has reached
its alloted cpu time.

*/



  struct timeval start, end;
  long sleep_time;

  int check_time()
  {

      long mtime, secs, usecs;    

          gettimeofday(&end, NULL);
          secs  = end.tv_sec  - start.tv_sec;
          usecs = end.tv_usec - start.tv_usec;
          mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
          //printf("mtime %ld", mtime);
          ///printf("sleep time %ld", sleep_time);


        if (mtime > sleep_time)
        {
         // printf("we done");
          return 1;
        }


  };



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

  //dec_sleep_time = 3.2;

  dec_sleep_time = dec_sleep_time*1000; // converts to milli seconds

  sleep_time = dec_sleep_time; // casts to long
  


  gettimeofday(&start, NULL);

 

  // idea came from
  //https://stackoverflow.com/questions/21161175/example-of-very-cpu-intensive-c-function-under-5-lines-of-code
  int i, num = 1, primes = 0, check;

    while (num <= 1000000) 
    { 
        i = 2; 
        while (i <= num) { 

          
          check = check_time();

          if (check == 1)
          {
            return 1;

          }
          


            if(num % i == 0)
            {
              
              i++; 
              break;
              
          }
        }
        if (i == num)
            primes++;
        
        num++;
      
    }
  


  

  //usleep(sleep_time);
  //printf("done");
  return 0;
}

