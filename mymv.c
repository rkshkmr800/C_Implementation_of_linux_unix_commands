#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	char path[100],path_dest[100],present_path[100],temp_path[100];	//to store present path, temporary strings
	char **arg; //to store the command line arguments
	size_t size=100;	//specify buffer size

	getcwd(present_path,size);	//get present directory


	if(argc ==1)
	{
		printf("ERROR: no arguments specified\n");
	}
	else if(argc==2)
	{
		printf("ERROR: atleast two arguments are needed\n");
	}
	else if(argc==3)
	{
		arg = (argv+1);
		int f1=0,f2=0;	//flags to check whether two arguments are 

		strcpy(path,present_path);	//copy present directory to temp variable

		if(*arg!=NULL)
		{
			if(*arg[0]=='/')	// absolute path
				strcpy(path,*arg);
			else	// relativce path
			{
				strcat(path,"/");
				strcat(path,*arg);	//creating the path string
			}
			struct stat fileStat;	//strcuture to store the info of each member
			stat(path,&fileStat);   //info of each member

			if( access( path, F_OK ) != -1 ) 
			{
			    // file exists
			    f1 = (S_ISREG(fileStat.st_mode)) ? 0 : 1;	//directory or not
			} 
			else //error message
			{
			    // file doesn't exist
			    printf("no file : %s\n",path );
			    exit(0);
			}
			
		}

		arg++;	//next argument

		strcpy(path_dest,present_path);	//copy present directory to temp variable

		if(*arg!=NULL)
		{
			if(*arg[0]=='/')	// absolute path
				strcpy(path_dest,*arg);
			else	// relativce path
			{
				strcat(path_dest,"/");
				strcat(path_dest,*arg);	//creating the path string
			}
			struct stat fileStat;	//strcuture to store the info of each member
			stat(path_dest,&fileStat);   //info of each member

			if( access( path_dest, F_OK ) != -1 ) //when file exists we need to move
			{
			    // file exists
			    f2 = (S_ISREG(fileStat.st_mode)) ? 2 : 1;	//directory or not
			    if(f2==1)
			    {
			    	strcat(path_dest,"/");
					strcat(path_dest,basename(path));	//creating the path string
			    }
			} 
			else
				f2 = (S_ISREG(fileStat.st_mode)) ? 0 : 1;	//directory or not
		}

		if(f1 == f2 || (f1==0 && f2==1))	//when two files or two directries or file & directory
		{
			rename(path, path_dest);	//rename or swap
		}
		else if(f2==2)
		{
			printf("mymv: cannot overwrite '%s' with file '%s' which already exists\n",basename(path_dest),basename(path));

		}
		else	//error in order of arguments
		{
			printf("mymv: cannot overwrite non-directory '%s' with directory '%s'\n",basename(path_dest),basename(path));
		}
	}
	else 
	{

		strcpy(path_dest,present_path);	//copy present directory to temp variable
		strcat(path_dest,"/");
		arg = (argv+(--argc));

		strcat(path_dest,*arg);	//creating the path string
		strcat(path_dest,"/");	//creating the path string

		argc--;
		for (arg = ++argv ; arg != argv+argc ; arg++) 
		{
			strcpy(path,present_path);	//copy present directory to temp variable

			if(*arg!=NULL)
			{
				if(*arg[0]=='/')	// absolute path
					strcpy(path,*arg);
				else	// relativce path
				{
					strcat(path,"/");
					strcat(path,*arg);	//creating the path string
				}
				struct stat fileStat;	//strcuture to store the info of each member
				stat(path,&fileStat);   //info of each member

				if( access( path, F_OK ) != -1 ) 
				{
				    strcpy(temp_path,path_dest);	//copy present directory to temp variable
					strcat(temp_path,*arg);	//creating the path string
					rename(path, temp_path);	//rename or swap
				} 
				else //error message
				{
				    // file doesn't exist
				    printf("no file : %s\n",path );
				}
				
			}
		}
	}
}
