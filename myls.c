#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char **argv)
{
	char path[100],present_path[100];	//to store present path
	char **arg; //to store the command line arguments
	size_t size=100;	//specify buffer size
	DIR* dir;	//directory pointer
	struct dirent *sd=NULL;	//pointer for each member of directory
	struct direct **files; //each member list
	int i,j;

	char *months[12] = {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec" };	//array to encode number to month

	getcwd(present_path,size);	//get present directory

	for (arg = ++argv ; arg != argv+argc ; arg++) {

		strcpy(path,present_path);	//copy present directory to temp variable

		if(*arg!=NULL && !strcmp(*arg,"-l"))
		{
			arg++;
			if(*arg!=NULL)	// when argument is given
			{
				if(*arg[0]=='/')	//absolute path
					strcpy(path,*arg);
				else	//convert relative path to absolute path
				{
					strcat(path,"/");
					strcat(path,*arg);
				}

			}

			//open the directory required
			dir=opendir(path);

			if(dir)	//if directory exists
			{
				i =0 ;
				scandir(path, &files, NULL, alphasort);

		        while((sd=*files++)!=NULL)	//iterate over each member of directory
		        {

		            struct stat fileStat;	//strcuture to store the info of each member
				    stat(sd->d_name,&fileStat);   //info of each member

				    i++;
		        	if(i<=2)	//to skip the default directories of unix
		        	{
		        		continue;
		        	}

				    //permissions info
				    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");	//directory or not
				    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");	//user read
				    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");	//user write
				    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");	//user execute
			        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");	//group read
			        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");	//group write
			        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");	//group execute
			        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");	//public read
			        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");	//public write
			        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");	//public execute

			        printf(" %2ld",fileStat.st_nlink);	//no. of items asscoiated to

			        struct passwd *pwd;
			        pwd = getpwuid(fileStat.st_uid);
			        printf("%8s",pwd->pw_name);	//username of owner

			        struct group *grp;
			        grp = getpwuid(fileStat.st_gid);
			        printf("%8s",grp->gr_name);	//user group

			        printf(" %6ld",fileStat.st_size);	//size of file or directory

			        printf(" ");
			        struct tm info = *localtime(&fileStat.st_mtime);
			        //converting into time format
			        printf("%s %2d %2d:%2d",months[info.tm_mon],info.tm_mday,info.tm_hour,info.tm_min);

			        printf(" ");
			        printf("%s",sd->d_name);	//name of the member

			        printf("\n");
			 
				}
				closedir(dir);//close directory after usage
	        }
	        else
			{
				printf("No such directory exists : %s\n",path);
			}
		}
		else
		{
			if(*arg!=NULL)	//if arg is null present directory else the argument
			{
				if(*arg[0]=='/')	//if absolute path
					strcpy(path,*arg);
				else	//else create abs path for directory specified in argument
				{
					strcat(path,"/");
					strcat(path,*arg);	//absolute path
				}

			}

			dir=opendir(path);	//open directory
			if (dir)	//if dircetory exists
			{
				i = 0;
				j = 0;

				scandir(path, &files, NULL, alphasort);

		        while((sd=*files++)!=NULL)	//iterate over each member of directroy
		        {
		        	i++;
		        	if(i<=2)	//to skip the default directories of unix
		        		continue;
		        	if(strcmp(sd->d_name,""))
		            	printf("%s\n",sd->d_name);

		        }
				closedir(dir);	//close directory at end of usage
			}
			else	//directory does not exist
			{
					printf("No such directory exists : %s\n",path);
			}

		}
		
		break;
	}
}
