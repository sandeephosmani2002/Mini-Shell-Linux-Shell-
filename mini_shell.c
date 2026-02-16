#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include "header.h"
#include <sys/wait.h>


void extract_external_commands(char *external_commands[153])
{
     int fd = open("minishell_command.txt",O_RDONLY);

     if(fd == -1)
     {
	     perror("open");
	     return ;
     }

     char ch;
     char command[20];
     int i;
     for(i=0;i<153;i++)
     {

            int lenght_command = 0;
            int ret = read(fd,&ch,1);
	    if(ch != '\n')
	    {

		    while(ch != '\n')
		    {
			    command[lenght_command++] = ch;
			    read(fd,&ch,1);
		    }
		    command[lenght_command] = '\0';
	    }
            
	    external_commands[i] = malloc(strlen(command));
	    strcpy(external_commands[i],command);
     }

     external_commands[i] = NULL;

  

}


char *get_command(char *input_string)
{
    int i = 0, j = 0;

    // 1) skip starting spaces
    while (input_string[i] == ' ')
    {
        i++;
    }

    // allocate memory
    char *command_str = malloc(strlen(input_string) + 1);

    // 2) copy command until space or end
    while (input_string[i] != ' ' && input_string[i] != '\0')
    {
        command_str[j] = input_string[i];
        i++;
        j++;
    }

    // 3) null terminate
    command_str[j] = '\0';

    return command_str;
}


int check_command_type(char *command,char *external_commands[153],char *builtins[])
{
     int i=0;
     while(builtins[i]!= NULL)
     {
	     if(strcmp(builtins[i],command) == 0)
	     {
		     return BUILTIN;
		     break;
	     }
	     i++;
	     
     }

     int j=0;
     while(external_commands[j] != NULL)
     {
	     if(strcmp(external_commands[j],command) == 0)
	     {
		     return EXTERNAL; 
		     break;
	     }
	     j++;
			     
     }

     return NO_COMMAND; 
}
