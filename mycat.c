#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
Name : print_cat

arguments:
path : path of file whose contents to display

description: To display file contents on to the console
*/
void print_cat(char path[100])
{
	FILE *ptr = fopen(path,"r"); //opening file
	if(ptr)
	{
		fseek(ptr, 0, SEEK_END);
		long fsize = ftell(ptr);	//get size of the file contents
		fseek(ptr, 0, SEEK_SET);  //same as rewind(ptr);

		char *data = malloc(fsize + 1);
		fread(data, fsize, 1, ptr);
		fclose(ptr);	//close after usage 
		printf("%s\n",data);	//display on terminal
	}
	else
		printf("%s : No such file or directory\n",path);	//no file present
};
int main(int argc, char **argv)
{
	char path[100],present_path[100];	//to store present path
	char **arg; //to store the command line arguments
	size_t size=100;	//specify buffer size
	char * line = NULL;	//line ponter
	size_t len = 0;	//len of line
	ssize_t read;	//read line from file variable

	getcwd(present_path,size);	//get present directory

	for (arg = ++argv ; arg != argv+argc ; arg++) {

		strcpy(path,present_path);	//copy present directory to temp variable

		if(*(argv)==NULL)	//when no argument is specified
		{
			while ((read = getline(&line, &len, stdin)) != -1)	//read line by line 
			{
				if(!(strcmp(line,"myquit\n")))
					break;
				printf("%s", line);	//display lines
			}
		}

		if(*arg!=NULL)
		{
			if(*arg[0]=='/')	// absolute path
				strcpy(path,*arg);
			else	// relative path
			{
				strcat(path,"/");
				strcat(path,*arg);	//creating the path string
			}
			print_cat(path);	//call function
		}
	}
}
