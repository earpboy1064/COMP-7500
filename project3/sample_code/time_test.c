#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main() {

time_t rawtime;
  struct tm * timeinfo;
  time( &rawtime );
  timeinfo = localtime( &rawtime );
  printf("%02d:%02d:%02d", timeinfo->tm_hour -5 , timeinfo->tm_min, 
    timeinfo->tm_sec);
   

    float total_time = (((timeinfo->tm_hour -5) * (60*60)) +  (timeinfo->tm_min * 60) + timeinfo->tm_sec);

    printf("\ntotal_time: %f", total_time);

    int hours = total_time/3600;
    printf("\nhours: %d\n", hours);

     return 0;
}

