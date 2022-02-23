#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 5000

/*
/  COMP 7500 
/  Wyatt LeMaster
/  Auburn University
/  
/ This block explains how to compile and run the pwordcount.c file
/  
/ how to compile?
/ $ gcc -g -lm -o pwordcount pwordcount.c 
/
/ How to run?
/
/ Run using a single argument.
/ This argument should be a file in the format of < file.txt > 
/ 
/ an example is
/ 
/ ./pwordcount words.txt
/
/ The program will then print out the word count
/
/ The below program uses two different pipes to count the words in a text file
/ you can read about each function in the code blocks inside of all 3 functions
/ 
/ function 1 is parent
/ function 2 is child
/ function 3 is word count  
/
/ Explanation:
/ 
/ The parent will open and divide up a text file. 
/ sending chunks of the text file to the child
/ The child will use a function to count the words in each chunk
/ and send the total back to the parent
/ 
*/

// Function prototypes
void parent(char file_name[], int pipe_one_fd[], int pipe_two_fd[]);

int child(int pipe_one_fd[], int pipe_two_fd[]);

int word_count(char file[]);

int main(int argc, char *argv[])
{

  // Checks to make sure user entered filename
  if (argc == 1)
  {
    printf("\n\nPlease enter a file name.");
    printf("\nUsage: ./pwordcount<file_name>\n\n");
    return 0;
  }

  // opening two pipes
  int pipe_one_fd[2];
  int pipe_two_fd[2];
  pid_t child_pid;

  //Pipe descriptors and error checking
  //*******************************************************
  int pipe_one_return = pipe(pipe_one_fd);

  // Checks to see if pipe creation failed
  if (pipe_one_return == -1)
  {
    printf("Failed to open pipe one");
    return 1;
  }

  // Same for pipe 2
  int pipe_two_return = pipe(pipe_two_fd);

  if (pipe_two_return == -1)
  {
    printf("Failed to open pipe two");
    return 1;
  }

  //******************************************************* 

  //Creating child and calling functions
  //*******************************************************
  // Each function call includes both pipes 
  // The parent includes the file name

  child_pid = fork();	// Forking to create child process

  if ((child_pid) != 0)
  {
    parent(argv[1], pipe_one_fd, pipe_two_fd);
  }
  else
  {
    child(pipe_one_fd, pipe_two_fd);
  }

  return 0;
}

// END OF MAIN

//****************************************************************

// START OF FUNCTION DEFINITIONS

//PARENT 
void parent(char file_name[], int pipe_one_fd[], int pipe_two_fd[])
{
  /*
#
#
# This is the parent function.
# It opens a file and reads the content storing it 
# in a string to be easily sent to the child function. 
#
# it only reads a certain amount of data determined by the const BUFFER_SIZE 
# this allows it to handle large files
# because it chops it up and processes it in bite size chunks 
#
# to prevent chopping a word in half and mistakenly reading it twice  
# it will find a suitable stopping point 
# that is a space before sending the chunk to the child 
#
#
# It sends each chunk to the child. When a response is heard it prints the 
# response containing the word count
#
#
*/

  FILE *fp = fopen(file_name, "r");
  char exit_code[] = "";

  // This will close every end of the pipes incase the file cannot be read
  if (fp == 0)	
  {
    printf("\nFailed to open file\n");
    printf("Please check file name for spelling errors\n\n");

     

    // Sends exit code of a empty char to the child
    write(pipe_one_fd[WRITE_END], &exit_code, BUFFER_SIZE);

    close(pipe_one_fd[READ_END]);
    close(pipe_one_fd[WRITE_END]);
    close(pipe_two_fd[WRITE_END]);
    close(pipe_two_fd[READ_END]);
  }
  else
  {
    printf("\nfilename: (%s) loaded \n", file_name);

    char chunk[BUFFER_SIZE]; // The chunk that will hold a portion of the file

    close(pipe_one_fd[READ_END]); // Close read end of pipe 1

    char c = ' '; 
    while (c != EOF)
    {
      int i = 0;

      // This is needed because residual elements can be left over.
      memset(chunk, 0, BUFFER_SIZE);	// Clears the array, 

      // This loop finds a space and breaks the string to prevent 
      // Breaking a word in half causing errors in word counting
      for (i; i < BUFFER_SIZE; i++) 
      {                               
        
        c = fgetc(fp);
        if (i > BUFFER_SIZE - 50)
        {
          if (isspace(c))
          {
            chunk[i] = c;
            break;
          }
        } 

        if (c == EOF)
        {
          break;
        }
          chunk[i] = c;
      }

      printf("\nparent(%d) Writing chunk to pipe.\n", getpid());

      // Writes chunk to the pipe then waits
      // for 1 second for child to process 
      write(pipe_one_fd[WRITE_END], &chunk, strlen(chunk));
      sleep(1);
    }

  
    // Sends exit code of a empty char to the child
    write(pipe_one_fd[WRITE_END], &exit_code, BUFFER_SIZE);

    close(pipe_one_fd[WRITE_END]);  // Close write end of pipe 1

    // PIPE 2
    close(pipe_two_fd[WRITE_END]);  // Closes the write end of pipe 2
    int total_word_count = 0;

    // Now read the data (will block)
    // reads word count from child
    read(pipe_two_fd[READ_END], &total_word_count, sizeof(total_word_count));

    printf("\nparent(%d) received value",getpid());
    printf(" word count = %d\n", total_word_count);

    close(pipe_two_fd[READ_END]);	// Closes the read end of pipe 2 

  }
}

//****************************************************************

// Child function definition
int child(int pipe_one_fd[], int pipe_two_fd[])
{

/*Child explanation
#
#
# The child gets a string in the form of a chunk from the parent
# the child then passes this chunk to a 
# word count function which then counts the words in the chunk and
# passes it back to the child.
# The child adds the word count to a running total
# and when it receives an exit code from the parent 
# which is an empty char it then returns the total word count.
#
#
*/

  char buf[BUFFER_SIZE];	// Buffer to hold the chunks 

  int total_word_count = 0;

  // Child: reading only, so close the write-descriptor
  close(pipe_one_fd[WRITE_END]);

  // Now read the data (will block)
  read(pipe_one_fd[READ_END], buf, BUFFER_SIZE);

  if (strlen(buf) == 0)	// Ends child process if parent fails to send info 
  {
    close(pipe_one_fd[WRITE_END]);
    close(pipe_one_fd[READ_END]);
    close(pipe_two_fd[READ_END]);
    close(pipe_one_fd[READ_END]);
    exit(0);
  }

  // This will keep reading from the pipe until the string length is 0
  while (strlen(buf) != 0) 
  {
    // Note: This string is sent from the parent and is called exit_code

    // Calls function to count words
    total_word_count += word_count(buf);

    // Clears buf to prevent words from previous chunk to remain
    memset(buf, 0, BUFFER_SIZE);	
    // Now read the data (will block)
    read(pipe_one_fd[READ_END], buf, BUFFER_SIZE);
    printf("\nChild(%d) Received Chunk from parent\n", getpid());
  }

  close(pipe_one_fd[READ_END]);	// Close the read-descriptor

  //PIPE 2
  close(pipe_two_fd[READ_END]);	// Close read end of pipe 2

  // Writes the word count to the pipe to be returned to parent
  write(pipe_two_fd[WRITE_END], &total_word_count, sizeof(total_word_count));
  close(pipe_one_fd[READ_END]);	// Close write end of pipe 1
}

//****************************************************************

// Word count function 
int word_count(char file[])
{

/*
#
# This function receives a string and counts the spaces in the
# string to deduce the amount of words.
#
# To avoid errors from accidental spaces the function 
# considers if the last char it read was a space or not.
# If it was a repeated space it does not consider
# the next spaces to be words. 
# This continues until the isspace() fails and resets the variable.
#
*/

  int i = 0;
  int word_count = 0;
  int last_was_char = 0;
  char c = file[i];

  // Loops for the length of the string counting spaces
  for (i = 0; i < strlen(file) + 1; i++)	
  {

    if (isspace(c))
    {
      // If the last char read was a char and not a space
      if (last_was_char == 1) 
      {
      // It will add a word to the total and reset last_was_char
        word_count = word_count + 1;
        last_was_char = 0;
      } 
    }
    else
    {
      last_was_char = 1;
    }
    c = file[i];
  }

  return word_count;
}
//**************************** END OF PROGRAM ******************************
