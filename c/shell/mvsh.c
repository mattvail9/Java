/* Matthew Vaillancourt - HW3*/
/* A program that simulates a simple shell */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

struct Node{
	char *data;
	struct Node *next;
};

struct Node *head = NULL;
struct Node *current = NULL;

void printPrompt();
char *readCommandLine();
char *parseCommand(char *cmdLine);
int isInternalCommand(char *cmd);
void executeInternalCommand(char *cmd);
void executeCmd(char *cmd);
int mkdir(const char *pathname, mode_t mode);
int chdir(const char *path);
void executeCommand(char *cmd);

/* standard buffer size */
int BUFFSIZE = 64;
/* Internal shell commands */
char *commands[6] = {"cd", "help", "exit", "pwd", "access", "history"};				
int numCommands = 6;

/* array for tokenized user input */
char *args[3];
int MAXARGS = 3;
int status = 0;

int main(int argc, char **argv)
{
	pid_t pid;
	pid_t child;

	while(1)
	{
		char *cmdLine = NULL;	/* Stores user input */
		char *cmd = NULL;		/* Stores tokenized command */

		printPrompt();
		cmdLine = readCommandLine();

		/*printf("Before: %s\n",cmdLine);*/
		cmd = parseCommand(cmdLine);

		/* printf("arg[0]: %s..\narg[1]: %s..\nargs[2]: %s..\n",args[0],args[1],args[2] ); */
		if(isInternalCommand(cmd))
		{
			executeInternalCommand(cmd);
		}
		else
		{
			pid = fork();							
			if(pid == 0) /* child process */
			{
				executeCommand(cmd);
			}
			else if(pid > 0) /* parent process */
			{
				/* wait for child to complete */
				child = waitpid(pid, &status, 0);
			}
			else
			{
				printf("%s%d\n","Child pid: ",child);
				printf("an error has ocurred\n");
			}
		}
	}
}
void executeCommand(char *cmd)
{
	execvp(cmd, args);
}

/* Prints prompt to stdout */
void printPrompt()
{
	printf("MV>");
}

char *readCommandLine()
{
	char *cmdLine = NULL;
	size_t size = 0;
	size_t length = 0;
	size_t end = 0;

	do {
		/* add BUFFSIZE to current size */
		size += BUFFSIZE;
		cmdLine = realloc(cmdLine, BUFFSIZE);
		/* read user input */
		fgets(cmdLine + end, size, stdin);
		length = strlen(cmdLine);
		/*last char before null char */
		end = length - 1;
	} while (cmdLine[end] != '\n');
	
	if(strlen(cmdLine) > 0)
	{		
		if(cmdLine[0] == '!')
		{
			if(strcmp(cmdLine,"!!") == 0)
			{
				//execute last command
			}
			else
			{
				/* Test For Integers*/
				char *ptr = cmdLine;
				while (*ptr) 
				{ 
	    			if ( isdigit(*ptr) ) 
	    			{
				        /* Found a number */
				        int val = (int)strtol(ptr,&ptr,10);
				        printf("%d\n", val);
			    	} 
				    else 
				    {
				        ptr++;
				    }
				}
			}
		}
		else if(head == NULL){
			head = malloc(sizeof(struct Node));
			head->data = malloc(strlen(cmdLine+1));
			strcpy(head->data,cmdLine);
			current = head;
		}
		else{
			struct Node *ptr = malloc(sizeof(struct Node));
			ptr->data = malloc(strlen(cmdLine+1));
			strcpy(ptr->data,cmdLine);
			current->next = ptr;
			current = current->next;
		}
	}
	return cmdLine;
}

/* extracts arguments from input */
/* returns 1st string in input */
char *parseCommand(char *cmdLine)
{
	/* remove trailing newline char */
	cmdLine[strcspn(cmdLine, "\n")] = 0;
	args[0] = strtok(cmdLine, " ");
	int i = 1;
	while(cmdLine[0] != '\0' && i < MAXARGS){
		args[i] = strtok(NULL, " ");
		i++;
	}				
	return args[0];
}

/* compares cmd against list of internal commands */
int isInternalCommand(char *cmd)
{
	int i;
	for(i = 0; i < numCommands; i++){
		if((strcmp(commands[i], cmd)) ==0){
			return 1;
		}
	}
	return 0;
}

/* Executes internal shell commands */
void executeInternalCommand(char *cmd)
{
	if((strcmp(commands[0], cmd)) ==0){	/* cd */
		if(args[1] == NULL || strcmp(args[1], "~") == 0){
			/* if no arg or ~ then access environment variable $HOME */
			args[1] = getenv("HOME");
		}
		chdir(args[1]);
	}
	else if((strcmp(commands[1], cmd)) ==0){	/* help */
		printf("%s\n", "Listing of all internal commands");
		printf("%-10s%-10s%s\n","cd","[dir]","changes working directory to dir");
		printf("%-10s%-10s%s\n","help","","prints a list of all internal commands");
		printf("%-10s%-10s%s\n","exit","","causes normal process termination");
		printf("%-10s%-10s%s\n","pwd","","prints current working directory");
		printf("%-10s%-10s%s\n","mkdir","[dir]","makes a directory dir");
		printf("%-10s%-10s%s\n","access","[file]","checks user access to file");
	}
	else if((strcmp(commands[2], cmd)) ==0){	/* exit */
		printf("Exit\n");
		exit(0);
	}
	else if((strcmp(commands[3], cmd)) ==0){	/* pwd */
		char buf[1028];
		printf("%s\n", getcwd(buf, 1028));
	}
	else if((strcmp(commands[4], cmd)) ==0){	/*access*/
		if(access(args[1], F_OK) > -1){
			printf("Access OK\n");
		}
		else
		{
			printf("Cannont access file\n");
		}
	}
	else if(strcmp(commands[5], cmd) == 0)
	{
		if(head == NULL)
		{
			printf("%s\n", "History is empty");
		}
		else{
			int i = 0;
			struct Node *temp;
			temp = head;
			while(temp != NULL){
				printf("%0d %s", i, temp->data);
				i++;
				temp = temp->next;
			}
		}
	}
}
