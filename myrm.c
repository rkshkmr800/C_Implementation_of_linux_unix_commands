#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>

/*

function to recursively delete directories
arguments:
present_path: directory path to recursively delete (full path)

*/
void recursive_delete_directory(char present_path[100])
{
	struct dirent *sd=NULL;	//pointer for each member of directory
	struct direct **files; //each member list
	int i;
	char path[100];

	scandir(present_path, &files, NULL, alphasort);	//scan files and directories

	while((sd=*files++)!=NULL)	//iterate over each member of directroy
	{
		i++;
		if(i<=2)	//to skip the default directories of unix
			continue;

		if(strcmp(sd->d_name,".") && strcmp(sd->d_name,".."))
		{
			strcpy(path,present_path);	//copy present directory to temp variable
			strcat(path,"/");
			strcat(path,sd->d_name);

			struct stat fileStat;	//strcuture to store the info of each member
			stat(path,&fileStat);   //info of each member

			if( access( path, F_OK ) != -1 ) 
			{
				DIR* directory = opendir(path);

				if(directory)
				{
					recursive_delete_directory(path);	//if directory call again
				}
				else
				{
					int ret = remove(path);	//simple file
					if(ret) 
					{
						printf("Error: unable to delete the file : %s\n",path);
					}
				}

				closedir(directory);	//close directory at end of usage
			}
		}

	}

	remove(present_path);
};

int main(int argc, char **argv)
{
	char path[100],path_dest[100],present_path[100],temp_path[100];	//to store present path, temporary strings
	char **arg; //to store the command line arguments
	size_t size=100;	//specify buffer size
	struct dirent *sd=NULL;	//pointer for each member of directory
	struct direct **files; //each member list
	int i,f;
	getcwd(present_path,size);	//get present directory

	if(argc ==1)
	{
		printf("myrm: missing operand\n");
	}
	else
	{
		arg = (argv+1);
		if(!strcmp(*arg,"-r"))
		{
			for (arg = (argv+2); arg != argv+argc ; arg++) 
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

			    		//int f = (S_ISREG(fileStat.st_mode)) ? 0 : 1;	//directory or not
			    		DIR* directory = opendir(path);
			    		if(directory)
					    {
							recursive_delete_directory(path);	//recursive call
					    }
					    else
					    {
							int ret = remove(path);	//simple file to delete
						   	if(ret) 
							{
							    printf("Error: unable to delete the file : %s\n",path);
							}
						}
						closedir(directory);
					} 
					else //error message
					{
					    // file doesn't exist
					    printf("no file : %s\n",path );
					}
					
				}
			}
		}
		else
		{
			for (arg = (argv+1); arg != argv+argc ; arg++) 
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

			    		//int f = (S_ISREG(fileStat.st_mode)) ? 0 : 1;	//directory or not
			    		DIR* directory = opendir(path);
			    		if(directory)
					    {
							scandir(path, &files, NULL, alphasort);

							i =0;
							f = 0;

					        while((sd=*files++)!=NULL)	//iterate over each member of directroy
					        {

					        	i++;
					        	if(i<=2)
					        		continue;
					        	f=1;
					        	break;
					        }

					        if(f==0)	// simple file
								remove(path);
					        else	//directory
					        	printf("myrm: cannot remove '%s': Is a non-empty directory\n", basename(path));
					    }
					    else
					    {
							int ret = remove(path);	//simple file to delete
						   	if(ret) 	
							{
							    printf("Error: unable to delete the file : %s\n",path);
							}
						}
						closedir(directory);
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
}
