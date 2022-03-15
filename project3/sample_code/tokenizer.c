#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/types.h>

# define MAXMENUARGS 5

int cmd_dispatch(char *cmd) {

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
		  //	return E2BIG;
		}
		args[nargs++] = word;
	}

	if (nargs==0) {
		return 0;
	}

for(i = 0; i < nargs; i++)
{
printf("Word %d contents %s\n", i, args[i]);

}

}

// ^^^  Get this working boy
int main( int argc, char *argv[])
{
  int word_count = 0;
  int last_was_char = 0;
  int i = 0;
    
    int arg_counter = 0;
    char str[] = {"process 1 2 3"}; 
    char temp[100] = {" "};
    char c;
    //c = str[i];

    char name[50];
    char priority[2];
    char time[5];

    printf("string test: %s\n", str);

    cmd_dispatch(str);

};

/*
// Loops for the length of the string counting spaces
  for (i = 0; i < strlen(str) + 1; i++)	
  {

    if (isspace(c))
    {
      // If the last char read was a char and not a space
      if (last_was_char == 1) 
      {
      // It will add a word to the total and reset last_was_char
        word_count = word_count + 1;
        last_was_char = 0;
        arg_counter++;
        if (arg_counter == 1)
        {
         //strcat(name, temp);
        }
      } 
    }
    else
    {
      last_was_char = 1;
      printf("char: %c\n", c);
     // my_args[arg_counter] = c
      temp[i] = c;
      temp[i+1] = '\0';
    }
    c = str[i];
  }
  printf("\nstring count: %d\n", word_count);
  printf("\nstring temp: %s\n", temp);
  printf("\nname: %s\n", name);

}
*/