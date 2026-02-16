
/*
 * NAME = SANDEEP HOSMANI
 * DATE = 08/02/26
 * mini_shell Project
 * -------------------
 * This file contains:
 * 1) Main shell loop (reads input, checks command type)
 * 2) Fork-exec for external commands
 * 3) Built-in command execution
 * 4) Signal handling (Ctrl+C, Ctrl+Z, background cleanup)
 *
 * Supported features:
 * - Built-in commands (cd, pwd, echo, exit, jobs, fg, bg, clear)
 * - External commands using execvp()
 * - Job control using SIGTSTP, SIGCONT
 * - Pipe execution (cmd1 | cmd2 | cmd3)
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include "header.h"

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "clear", "eval",
                                                "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","fg","jobs","bg",NULL};

// Linked list head for stopped jobs (Ctrl+Z)
s_list* head = NULL;

// Prompt string shown in terminal
char prompt_string[50] = "mini_shell:~";

// Stores currently running child PID (used by signal handler)
pid_t pid;

// Stores child process status for waitpid()
int status = 0;

// Stores user input command line
char input_string[50];

// Number of stopped jobs currently in job list
int stopped_count;


/*
 * main()
 * ------
 * Steps:
 * 1) Load external commands list from file
 * 2) Register signal handlers
 * 3) Display prompt and read user input continuously
 * 4) Identify command type (builtin / external)
 * 5) Execute based on type
 */

int main()
{
	// two array for the external commands
	char* external_commands[153];

	extract_external_commands(external_commands);

	//Register the signals
	signal(SIGINT,my_handler);
	signal(SIGTSTP,my_handler);

	system("clear");

	
        
	

        while(1)
	{
		// print the prompt of minishell 
	        printf(ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET"$",prompt_string);
		
		bzero(input_string, strlen(input_string));		
	
		// taking the input from terminal and validating the input 
		scanf("%[^\n]",input_string);
		getchar();

		char* PS1 = strstr(input_string,"PS1=");
	
		// if no input is passed then the print only prompt
                if(strlen(input_string)==0)
		{
			continue;
		}
		if(PS1 != NULL)
		{
			char* whitespace = strchr(input_string,' ');
			if(whitespace != NULL)
			{
				printf("Error : %s : command not found\n",whitespace);
				continue;
			}
			else
			{
				char* cmd = strchr(input_string,'=');

				strcpy(prompt_string,cmd+1);
				continue;
			}

		}
		

		// getting the command
		char *command = get_command(input_string);
 
		// checking the command type based on external and internal command
		int type = check_command_type(command,external_commands,builtins);
		
		if(type == EXTERNAL)  // external command
		{
		
                        pid = fork();

			if(pid>0)         // parent
			{
				waitpid(pid,&status,WUNTRACED);
				pid = 0;
			}
			else if(pid == 0)  // child
			{
				signal(SIGINT,SIG_DFL);
				signal(SIGTSTP,SIG_DFL);
				signal(SIGCHLD,SIG_DFL);
				execute_external_commands(input_string);
				exit(0);
			}
			else
			{
				perror("fork");
				exit(0);
			}

		}
		else if(type == BUILTIN)  // builtin command
		{

	                // execute the internal command
			execute_internal_commands(input_string);



		}
		else
		{

			printf("Error : NO command found!\n");
		}


	}


}

/*
 * my_handler()
 * ------------
 * Handles:
 * SIGINT  (Ctrl+C)  -> Do not kill shell, only stop current input or child
 * SIGTSTP (Ctrl+Z)  -> Stop current running child and store it in job list
 * SIGCHLD           -> Cleanup background child processes
 */

void my_handler(int signum)
{
	if(signum == SIGINT)
	{
		if(pid == 0)
		{
			if(!strlen(input_string))
			{


				printf(ANSI_COLOR_GREEN"\n%s"ANSI_COLOR_RESET"$",prompt_string);
				fflush(stdout);
			}
			else
			{
				printf("\n");
			}
		}
		else if(pid > 0)
		{
			printf("\n");
		}
	}
	else if(signum == SIGTSTP)
	{
		if(pid == 0)
		{
			if(!strlen(input_string))
			{
				printf(ANSI_COLOR_GREEN"\n%s"ANSI_COLOR_RESET"$",prompt_string);
				fflush(stdout);
			}
			else
			{
				printf("\n");
			}
		}
		else if(pid > 0)
		{
			// inserting the stoped input string to list
			insert_at_first(&head,pid,input_string);

			printf("\n[%d]+\tStopped\t\t%s\n",++stopped_count,input_string);
			fflush(stdout);
		}
	}
	else if( signum == SIGCHLD)
	{
		// clearing the resource the of child using WNOHANG
		waitpid(-1,&status,WNOHANG);
		signal(SIGCHLD,SIG_DFL);
	}

}

void insert_at_first(s_list **head,int pid,char* input_string)
{
	s_list *new = malloc(sizeof(s_list));
	if(new == NULL)
	{
		return;
	}
	strcpy(new->cmd,input_string);
	new->pid = pid;
	if(*head == NULL)
	{
	    *head = new;
	     return;
	}
	new->link = *head ;
	*head = new;


	return;
}

