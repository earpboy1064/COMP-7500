#include <stdio.h>
#include <ctype.h>

int main() {
  FILE *fp;

  fp= fopen("/home/project2/words.txt", "r");
  
  if (fp == NULL)
    {
      printf("cannot open file");
      
      
    }
  

  char c = fgetc(fp);
  
  int last_was_char = 0;

  int word_count = 0;
  while( c!= EOF)
    {
      if(isspace(c))
	{
	  if(last_was_char == 1){
	    word_count++;
	    last_was_char = 0;
	  }
	  
	}
      else
	{
	  last_was_char = 1;
	}

          printf("%c",c);
          //printf("%d", is_space);
          c = fgetc(fp);
      }
    

  printf("\nword count is: %d \n",word_count);

  printf("\nthis change was made in vsCode\n");
  fclose(fp);
  return 0;
}

