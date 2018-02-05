#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	char path[100],present_path[100];	//to store present path
	char **arg; //to store the command line arguments
	size_t size=100;	//specify buffer size

	getcwd(present_path,size);	//get present directory

	for (arg = ++argv ; arg != argv+argc ; arg++) {

		strcpy(path,present_path);	//copy present directory to temp variable

		if(*(argv)==NULL)
		{
			printf("mymkdir: missing operand\n");
			break;
		}
		if(*arg!=NULL)
		{
			if(*arg[0]=='/')	// absolute path
				strcpy(path,*arg);
			else	// relativce path
			{
				strcat(path,"/");
				strcat(path,*arg);	//creating the path string
			}
			int status = mkdir(path,0777);	//creating directory
			if(status)
			{
				printf("mymkdir: cannot create directory ‘%s’: File exists\n", basename(path));
			}
		}
	}
}
