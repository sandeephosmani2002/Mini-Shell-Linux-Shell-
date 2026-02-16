#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND      3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct node
{
	char cmd[50];
	int pid;
	struct node* link;
}s_list;


char *get_command(char *input_string);
void copy_change(char *prompt, char *input_string);
int check_command_type(char *command,char *external_commands[153],char *builtins[]);
void execute_external_commands(char *input_string);
void extract_external_commands(char **external_commands);
void execute_internal_commands(char *input_string);
int n_number_of_pip(int command_count, char *args[]);


void my_handler(int signum);
void print_list(s_list *head);
void sl_delete_first(s_list **head);
void insert_at_first(s_list **head,int pid,char* input_string);
void remove_extra_space_command(char* str);

#endif
