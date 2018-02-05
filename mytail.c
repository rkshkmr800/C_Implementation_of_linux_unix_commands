#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
function to print last num lines of file specified

arguments:
num : no. of lines from last to print
ptr : file pointer
flag :  to specify whether stdin (=0) or normal file(=1)
*/
void tail_print(int num, FILE *ptr,int flag)
{
	int i,j;
	char **lines;	//to store last num lines

	char * line = NULL;	//store line
    size_t len = 0;	//store len of line

	lines = calloc(num,sizeof(char *));	//allocate num no. of pointers to store last num lines

	for(i=0;i<num;i++)
		lines[i] = calloc(300,sizeof(char));	//create char array for each location
	i=0;

	if (flag == 0)	//case to read lines froms stdin
	{
		while (getline(&line, &len, stdin) != -1)	//read line by line 
		{
			if(!(strcmp(line,"myquit\n")))
				break;
			strcpy(lines[i],line); //copy into lines list
			i++;
			if(i == num)	// restart from 0
				i=0;
		}
	}
	else	//case to read lines from file if given
	{
		while (getline(&line, &len, ptr) != -1)	//read line by line 
		{

			strcpy(lines[i],line);
			i++;
			if(i == num)
				i=0;
		}
	}
	
	//printing the last 'num' lines, i indes points to last updated line , we need to print it circular	
	for(j=i;j<num;j++)	//from i to end
		printf("%s",lines[j]);
	
	for(j=0;j<i;j++)	//from start to i
		printf("%s",lines[j]);

};

int main(int argc, char **argv)
{
	char path[100],present_path[100];	//to store present path
	char **arg; //to store the command line arguments
	FILE *ptr;	//file pointer
	int num =10;	//default value of 10 for num of lines

	getcwd(present_path,100);	//get present directory

	arg = ++argv;

	if(*arg==NULL)
	{
		tail_print(num,NULL,0);	//stdin file
	}
	else
	{
    	char s[100] = "S";
    	strcpy(s,*arg);	//copy n value to string
    	if(s[0]=='-')
		{
			s[0] = '0';	//replace first char '-' with 0
			num = atoi(s);	//convert string to int
			if(!num)	//error msg
			{
				printf("mytail: enter proper argument\n" );
				_exit(0);
			}
			arg++;
		}
		if(*arg == NULL)
		{
			tail_print(num,NULL,0);	//stdin file
		}
		else
		{
			for ( ; *arg != NULL ; arg++) 
			{
				strcpy(path,present_path);

				if(*arg[0]=='/')	// absolute path
					strcpy(path,*arg);
				else	// relative path
				{
					strcat(path,"/");
					strcat(path,*arg);	//creating the path string
				}
				
				ptr = fopen(path,"r"); //opening file

				if(ptr)	//file exists then 
				{
					printf("===> %s <===\n", *arg);
					tail_print(num,ptr,1);	//normal file
				}
				else	//error msg
				{
					printf("%s : No such file or directory\n",*arg);	//no file present
				}
			}
		}
	}
}
