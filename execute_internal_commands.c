
/*
 * execute_internal_commands()
 * ---------------------------
 * Executes shell built-in commands.
 *
 * Built-ins:
 * - cd    : change directory using chdir()
 * - pwd   : print current directory using getcwd()
 * - echo  : supports $$, $?, $SHELL
 * - fg    : bring stopped job to foreground
 * - bg    : resume stopped job in background
 * - jobs  : print all stopped jobs
 * - clear : clear screen
 */

#include "header.h"
extern int status;
extern s_list* head;
extern int stopped_count;


void execute_internal_commands(char *input_string)
{
      char* command = get_command(input_string);
      if(strcmp(command,"cd") == 0)
      {
	      //remove extra space
	       
	      remove_extra_space_command(input_string);
	       char* input_path = input_string + 3;

	       if(chdir(input_path) == -1)
	       {
		       perror(input_path);
	       }
	       return;
      }
      else if(strcmp(command,"pwd") == 0)
      {
	      char pwd[50];
	      getcwd(pwd,sizeof(pwd));
	      printf(ANSI_COLOR_BLUE"%s\n"ANSI_COLOR_RESET,pwd);
	      return;
      }
      else if(strcmp(command,"exit") == 0)
      {
	      exit(0);
      }
      else if(strcmp(command,"echo") == 0)
      {
	      char *str = strstr(input_string,"$");
	      if(strcmp(str,"$$")==0)
	      {
		      int pid = getpid();
		      printf("%d\n",pid);

	      }
	      else if(strcmp(str,"$?")==0)
	      {
                     int exit_status = WEXITSTATUS(status);
		     printf("%d\n",exit_status);
	      }
	      else if(strcmp(str,"$SHELL")==0)
	      {
                    char *val = getenv("SHELL");
		    printf(ANSI_COLOR_BLUE"%s\n"ANSI_COLOR_RESET,val);
	      }

	      
      }                                                                
      else if(strcmp(command,"fg")==0)
      {
	      if(head == NULL)                                                               //fg command:
	      {                                                                              //* - Takes first stopped job from list
		      printf("-bash: fg: current: no such job\n");                           //* - Sends SIGCONT to resume
		      return;                                                                //* - Waits using waitpid(..., WUNTRACED)
	      }                                                                              //* - If job exits -> remove from list
                                                                                             //* - If job stops again -> keep it in list
	      s_list* job = head;
	      kill(head->pid,SIGCONT);
	      printf("%s\n",head->cmd);
	      waitpid(head->pid,&status,WUNTRACED);

	      
	          if (WIFEXITED(status) || WIFSIGNALED(status))
    		  {
        		// process finished → remove from job list
        		sl_delete_first(&head);
			stopped_count--;


		  }
                  //else if (WIFSTOPPED(status))
                  
                  // process stopped again → keep in list
                  // nothing to delete
                  

      }
      else if(strcmp(command,"bg")==0)                              //* bg command:
      {                                                             //* - Resumes the stopped job in background using SIGCONT
	      signal(SIGCHLD,my_handler);                           //* - Removes it from job list
	      if(head == NULL)                                      //* - SIGCHLD handler will cleanup when job finishes
	      {
		      printf("-bash: bg: current: no such job\n");
		      return;
	      }
	      
	      kill(head->pid,SIGCONT);
	      
	      printf("[%d] %s &\n", stopped_count,head->cmd);
	      sl_delete_first(&head);
	      stopped_count--;
      }
      else if(strcmp(command,"jobs")==0)
      {
             print_list(head);
	     
      }
      else if(strcmp(command,"clear")==0)
      {
	      system("clear");
      }


}

/*
 * sl_delete_first()
 * -----------------
 * Deletes the first node from job list.
 * Used after job finishes or moved to background.
 */

void sl_delete_first(s_list **head)
{
	if(*head == NULL)
	{
	    return;
	}
	s_list *temp = *head;
	*head = temp->link;
	free(temp);

	return;

}

/*
 * print_list()
 * ------------
 * Prints all jobs in job list in bash-like format:
 * [1]+ Stopped  command
 */

void print_list(s_list *head)
{
	if (head == NULL)
	{
		return;
	}
    else
    {
	    int i = 1;
	    while (head)
	    {
		    printf("[%d]+\tStopped\t\t%s\n",i,head->cmd);
		    head = head -> link;
		    i++;
	    }
	    

	    

	    
    }
}


/*
 * remove_extra_space_command()
 * ----------------------------
 * Removes extra consecutive spaces in input string.
 * Example:
 *   "cd     /home" -> "cd /home"
 */


void remove_extra_space_command(char* str)
{
    int i=0,j=0;
    while(str[i]!='\0')
    {
        if(str[i] ==' ' && str[i+1] == ' ')
        {
            j=i+1;
            while(str[j]!='\0')
            {
                str[j]=str[j+1];
                j++;
            }

        }
        else
        {
            i++;



        }
    }
}
