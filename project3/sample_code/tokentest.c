#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>
#include "tokenizer.h"

// ^^^  Get this working boy
int main( int argc, char *argv[])
{
  int word_count = 0;
  int last_was_char = 0;
  int i = 0;
    
    int arg_counter = 0;
    char str[] = {"quit\n"}; 
    char temp[100] = {" "};
    char c;
    //c = str[i];

    char name[50];
    char priority[2];
    char time[5];

    printf("string test: %s\n", str);
  int reslt;
  reslt = cmd_dispatch(str);

};
