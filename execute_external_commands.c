#include "header.h"

/*
 * execute_external_commands()
 * ---------------------------
 * Input:
 *  - input_string: full command line entered by user
 *
 * Function:
 * 1) Tokenizes input_string into args[] array
 * 2) Checks if pipe symbol '|' is present
 * 3) If no pipe -> execvp(args[0], args)
 * 4) If pipe present -> call n_number_of_pip()
 *
 * Note:
 * - execvp() replaces the current process image
 * - So this function does not return on success
 */


void execute_external_commands(char *input_string)
{

	char* temp = malloc(strlen(input_string)+1);
	strcpy(temp,input_string);

	// convert the 1D array to 2D array
	char* args[50];
	int count = 0;
	char *token = strtok(input_string," ");
	while(token != NULL)
	{
		args[count] = malloc(strlen(token)+1);
		strcpy(args[count],token);
		args[count][strlen(token)] = '\0';

		token = strtok(NULL," ");
		count++;
	}
	args[count] = NULL;

	int i=0,pipe_present = 0;

	while(args[i] != NULL)
	{
		if(strcmp(args[i],"|") == 0)
		{
			pipe_present = 1;
			break;
		}
		i++;

	}

	if(pipe_present == 0)
	{
		// execute commands withot the pip
		execvp(args[0],args);
	}
	else
	{
		n_number_of_pip(count,args);
	}


}


/*
 * n_number_of_pip()
 * -----------------
 * Executes multiple piped commands.
 *
 * Example:
 *   ls -l | grep txt | wc -l
 *
 * Steps:
 * 1) Count number of pipes
 * 2) Replace each "|" with NULL (to split commands)
 * 3) For each command:
 *    - create pipe()
 *    - fork()
 *    - child redirects stdout to pipe write end
 *    - parent redirects stdin to pipe read end
 * 4) execvp() each command one by one
 */

int n_number_of_pip(int command_count, char *args[])
{
	int pip_count = 0;

	for (int i = 1; i < command_count; i++)
		if (strcmp(args[i], "|") == 0)
			pip_count++;


	int arr[pip_count];
	int k = 0;

	for (int i = 0; i < command_count; i++)
	{
		if (strcmp(args[i], "|") == 0)
		{
			arr[k++] = i;
			args[i] = NULL;
		}
	}

	if(arr[0]==0)
	{
		printf("ERROR: '|' shouldn't be present at first\n");
		return 0;
	}

	if(arr[pip_count-1]==command_count-1)
	{
		printf("ERROR: '|' shouldn't be passed as last argument\n");
	}

	for(int i=0;i<pip_count;i++)
	{
		if(arr[i]-arr[i-1]==1)
		{
			printf("ERROR: '|' Shouldn't be passed consecutively\n");
			return 0;
		}
	}

	int pip[2];
	int indx = 0;

	for (int i = 0; i <= pip_count; i++)
	{
		if (i < pip_count)
			pipe(pip);

		int ret=fork();

		if (ret == 0)
		{
			if (i < pip_count)
			{
				dup2(pip[1], 1);
				close(pip[0]);
				close(pip[1]);
			}

			execvp(args[indx], &args[indx]);
			perror("execvp");
			_exit(1);
		}
		else if(ret>0)
		{
			if (i < pip_count)
			{
				dup2(pip[0], 0);
				close(pip[0]);
				close(pip[1]);
			}

			if (i < pip_count)
				indx = arr[i] + 1;
			wait(NULL);
		}
		else
		{
			perror("ERROR");
			return 0;
		}
	}

	return 0;
}
