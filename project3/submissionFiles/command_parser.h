

#define FCFS  1
#define SJF  2
#define PRIORITY  3





int cmd_dispatch(char *cmd, char* temp[]);
int cmd_check(char *args[], int total_args, int size_of_queue);
void commandline_parser(char *input_cmd);
void display_job_queue();
