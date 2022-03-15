#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>

# define MAXMENUARGS 5


/* Display menu information */
void showmenu(const char *name, const char *x[]) {
	int ct, half, i;

	printf("\n");
	printf("%s\n", name);
	
	for (i=ct=0; x[i]; i++) {
		ct++;
	}
	half = (ct+1)/2;

	for (i=0; i<half; i++) {
		printf("    %-36s", x[i]);
		if (i+half < ct) {
			printf("%s", x[i+half]);
		}
		printf("\n");
	}

	printf("\n");
}

static const char *helpmenu[] = {
	"[run] <job> <time> <priority>       ",
	"[quit] Exit AUbatch                 ",
	"[help] Print help menu              ",
      	NULL     /* Please add more menu options below */
};
int cmd_run(int nargs, char **args) {
	if (nargs != 4) {
		printf("Usage: run <job> <time> <priority>\n");
		return EINVAL;
	}
        
        /* Use execv to run the submitted job in AUbatch */
        printf("use execv to run the job in AUbatch.\n");
      	return 0; /* if succeed */
}

int cmd_quit()
{

  printf("please quit");
}
int cmd_helpmenu(int n, char **a) {
	(void)n;
	(void)a;
	showmenu("AUbatch help menu", helpmenu);
	return 0;
}
/* Command table. */
static struct {
	const char *name;
	int (*func)(int nargs, char **args);
} cmdtable[] = {
	/* commands: single command must end with \n */
	{ "?\n",	cmd_helpmenu },
	{ "h\n",	cmd_helpmenu },
	{ "help\n",	cmd_helpmenu },
	{ "r",		cmd_run },
	{ "run",	cmd_run },
	{ "q\n",	cmd_quit },
	{ "quit\n",	cmd_quit },
         {NULL, NULL}    /* Please add more operations below. */
};





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
			return E2BIG;
		}
		args[nargs++] = word;
	}

	if (nargs==0) {
		return 0;
	}

	for (i=0; cmdtable[i].name; i++) {
		if (*cmdtable[i].name && !strcmp(args[0], cmdtable[i].name)) {
			assert(cmdtable[i].func!=NULL);
            
            		result = cmdtable[i].func(nargs, args);
			return result;
		}
	}

	printf("%s: Command not found\n", args[0]);
	return EINVAL;
}



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