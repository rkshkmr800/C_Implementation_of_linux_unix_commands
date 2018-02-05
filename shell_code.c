#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
function to remove tab spaces or new line character from end of string
input; string
*/
void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
};

int main()
{
	char command[100],path[100],code_path[100],cmd_string[100];	//to store the command string, path of present working dir(all variables declared here)
	char *token,*temp_token;	//to store tokens splitted by space
	int pid;	// process id
	int status; // status of signals
	DIR* dir;	//directory pointer
	size_t size=100;

	getcwd(code_path,size);	//get present directory for executing codes
	strcat(code_path,"/");

	while(1)
	{
		printf("$PROMPT$>");
		gets(command);	//read user input

		char cmd_copy[100],cmd_temp_copy[100];	//store copies of command string
		strcpy(cmd_copy,command);	//copy command string
		strcpy(cmd_temp_copy,command); //copy command string

		token = strtok (command," ");

		if(!strcmp(token,"myexit"))	//to exit the prompt
		{
			printf("session terminated");
			kill(NULL, SIGTERM);	//kill the parent process
			break;
		}
		else if(!strcmp(token,"mycd"))
		{

			token = strtok (NULL, " ");
			if(token==NULL) //	cd needs arguments
			{
				printf("Error...no arguments\n");
			}
			else
			{
				if(token[0]=='/')	//absolute path
					strcpy(path,token);
				else	//relative path
				{
					getcwd(path,size);	//get present directory
					strcat(path,"/");
					strcat(path,token);	//append rel path
				}

				dir = opendir(path);	//check dir exists
				if (dir)
				{
					closedir(dir);	//close dir
					chdir(path);	//change directory function
				}
				else//error message
				{
					printf("No such directory exists : %s\n",path);
				}
			}
		}
		else
		{

			if((pid=fork())==-1)	//if unsuccessful child
			{
				continue;
			}		
			else if(pid>0)	//parent process
			{

				wait(NULL);	//wait for child
				kill(pid, SIGKILL);	//kill child
				continue;
			}
			else
			{
				strcpy(cmd_string,code_path);	//copy the source code directory

				int flag = -1;	//flag to check pipe command or not

				char cmd1[100],cmd2[100];	//two commands used in pipe command

				temp_token = strtok(cmd_temp_copy, "|");	//tokenizer
				strcpy(cmd1,temp_token);

				//iterate over tokenizer by pipe symbol
			    while( temp_token != NULL ) {
			    	flag++;
			    	strcpy(cmd2,temp_token);
			        temp_token = strtok(NULL, "|");
			        if(flag ==1) break;	//only execute commands of two pipes
			    }
			    
				if (flag == 1) //single pipe or not
				{
					
					char command_1[100],command_2[100];

					strcpy(command_1,code_path);	//copy the source directory
					strcpy(command_2,code_path);	//copy the source directory

				    int pd[2];	//declare ends of pipe
        			pipe(pd);	//create pipe


					if (!fork())
					{
						dup2(pd[1], 1); //create duplicate file pointer of stdout
						close(pd[0]);	//close other end of pipe

					    char *args[100];	//store cmd arguments
						int i = 0;

						token = strtok(cmd1, " ");	//tokenizer

						if(!strcmp(token,"mymkdir")||!strcmp(token,"mypwd")||!strcmp(token,"myls")||!strcmp(token,"mymv")||!strcmp(token,"myrm")||!strcmp(token,"myps"))
						{

							strcat(command_1,"./");
							strcat(command_1,token);	//creating the path of binary file to be executed

							while (token != NULL)	//iterate over command line arguments
							{

								args[i] = token;	//creating array of arguments
							    i++;

								token = strtok (NULL, " ");	//tokenizer
							}				
							args[i] = NULL;
							execve(command_1,args,NULL);
							_exit(0);
						}
						else if(!strcmp(token,"mycat")||!strcmp(token,"mytail"))	//skip if these commands
						{
							printf("");
							_exit(0);
						}
						else if(!strcmp(token,"myexit"))	//if exit command
						{
							_exit(0);
						}
						else	//when command is not present in system
						{
							printf("%s : command not found\n",cmd1);
							_exit(0);
						}
					} 
					else
					{
						wait(NULL);	//wait for child to finish

						dup2(pd[0], 0);	//create duplicate file pointer of stdin
        				close(pd[1]);	//close other end of pipe

						char *args[100];	//store cmd arguments
						int i = 0;

						token = strtok(cmd2, " ");	//tokenizer

						if(!strcmp(token,"mymkdir")||!strcmp(token,"mycat")||!strcmp(token,"mypwd")||!strcmp(token,"myls")||!strcmp(token,"mymv")||!strcmp(token,"mytail")||!strcmp(token,"myrm")||!strcmp(token,"myps"))
						{

							strcat(command_2,"./");
							strcat(command_2,token);	//creating the path of binary file to be executed

							while (token != NULL)	//iterate over command line arguments
							{

								args[i] = token;	//creating array of arguments
							    i++;

								token = strtok (NULL, " ");	//tokenizer
							}	

							args[i] = NULL;
						    execve(command_2,args, NULL);	//execute particular binary module
						    _exit(0);
						}
						else if(!strcmp(token,"myexit"))	//if exit command
						{
							_exit(0);
						}
						else	//when command is not present in system
						{
							printf("%s : command not found\n",cmd2);
							_exit(0);
						}
					}

				}
				else
				{
					temp_token = strtok(cmd_copy, " ");	//tokenizer with respect to space

					if(!strcmp(token,"mymkdir")||!strcmp(token,"mycat")||!strcmp(token,"mypwd")||!strcmp(token,"myls")||!strcmp(token,"mymv")||!strcmp(token,"mytail")||!strcmp(token,"myrm")||!strcmp(token,"myps"))
					{	

						char *args[100];	//store cmd arguments
						int i=0;

						strcat(cmd_string,"./");
						strcat(cmd_string,token);	//creating the path of binary file to be executed

						while (token != NULL)	//iterate over command line arguments
						{

							args[i] = token;	//creating array of arguments
						    i++;

							token = strtok (NULL, " ");	//tokenizer
						}				
						args[i] = NULL;

						execve(cmd_string,args, NULL);	//execute particular binary module
						_exit(0);

					}
					else if(!strcmp(token,"myexit"))	//if exit command
					{
						_exit(0);
					}
					else	//when command is not present in system
					{
						printf("%s : command not found\n",command);
					}
				}
			}
		}
	}
}
