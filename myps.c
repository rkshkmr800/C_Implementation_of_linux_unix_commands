#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pwd.h>
#include <math.h>

int main(int argc, char **argv)
{
	char path[100],present_path[100];	//to store present path
	char **arg; //to store the command line arguments

	size_t size=100;	//specify buffer size

	int uid = geteuid ();	//get user id
	int gid = getegid ();	//get group id

	struct dirent *sd=NULL;	//pointer for each member of directory
	struct direct **files; //each member list
	int i;	//counter

	FILE *fp;	//file pointer
	char * line = NULL;	//line ponter
	size_t len = 0;	//len of line
	ssize_t read;	//read line from file variable

	strcpy(path,"/proc/");
	strcat(path,"uptime");	//path to find the system uptime
	
	char uptime_str[100];
	fp=fopen(path,"r");	//open uptime file
	if(fp!=NULL)
	{
		getline(&line,&len,fp);
		sscanf(line,"%s ",uptime_str);	//get system uptime
	}
	long uptime = atof(uptime_str);	//string to float
	long Hertz = sysconf(_SC_CLK_TCK);	//get clock frequency

	scandir("/proc", &files, NULL, alphasort);	//scan files and directories

	printf("%6s %6s %s\n","   PID", "    TIME","CMD");

	while((sd=*files++)!=NULL)	//iterate over each member of directroy for process info
	{
		int num = atoi(sd->d_name);	//convert string to int
		if(num>0)
		{
			char file_path[100];	//store process info file path
			strcpy(file_path,"/proc/");	
			strcat(file_path,sd->d_name);	//process directory
			strcat(file_path,"/status");	//status file

			char name[100] = "", uid_str[100] = "", gid_str[100] = "", pid_str[100] = ""; //name of process, user id, group id, process id

			if( access( file_path, F_OK ) != -1 ) 
			{
				fp = fopen (file_path, "r");	//open file
				while ((read = getline(&line, &len, fp)) != -1)	//read line by line 
				{
			        sscanf(line, "Name:	%s\n", name);	//name of process
			        sscanf(line, "Uid:	%s\n", uid_str);	//user id
			        sscanf(line, "Gid:	%s\n", gid_str);	//group if
			        sscanf(line, "Pid:	%s\n", pid_str);	// process id
			    }
				fclose(fp);//close file
			}

			strcpy(file_path,"/proc/");
			strcat(file_path,sd->d_name);
			strcat(file_path,"/stat");	//path for time info the stat file

			fp=fopen(file_path,"r");	//open stat file
			
			getline(&line,&len,fp);	//single line to read

			// all the variables to extract the info from single line
			char comm[10],state;
			unsigned int flags;
			int pid,ppid,pgrp,session,tty_nr,tpgid;
			unsigned long minflt,cminflt,majflt,cmajflt,utime,stime;
			unsigned long long starttime;
			long cutime,cstime,priority,nice,num_threads,itreavalue;

			sscanf(line,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld  %ld %llu",&pid,comm,&state,&ppid,&pgrp,&session,&tty_nr,&tpgid,&flags,&minflt,&cminflt,&majflt,&cmajflt,&utime,&stime,&cutime,&cstime,&priority,&nice,&num_threads,&itreavalue,&starttime);
			
			unsigned long total_time=utime+stime;	//total time
			total_time=total_time+(unsigned long)cutime+(unsigned long)cstime;
			float seconds=uptime-(starttime/Hertz);
			float cpu_usage=100*((total_time/Hertz)/seconds);	//cpu usage

			int uid_num = atoi(uid_str);	//user id of process
			int gid_num = atoi(gid_str);	//group id ofprocess
    		
			int time = (int)(cpu_usage);	//cpu usage in integer value
			if (time < 0 ) time = 0 ;

			int hr = time/3600;	//hour
			int min = (time - (hr*3600))/60;	//minutes
			int sec = time - (min*60 + hr*3600);	//seconds

			char secs[100],hrs[100],mins[100],temp[100];	//string values of hour,minutes,seconds

			if(hr<9)	//less than 9 append 0 in 1st char
			{
				strcpy(hrs,"0");
				sprintf(temp, "%d", hr);
				strcat(hrs,temp);	//create hour string
			}
			else
				sprintf(hrs, "%d", hr);
			if(min<9) 	//less than 9 append 0 in 1st char
			{
				strcpy(mins,"0");
				sprintf(temp, "%d", min);
				strcat(mins,temp);	//create min string
			}
			else
				sprintf(mins, "%d", min);
			if(sec<9)	//less than 9 append 0 in 1st char
			{
				strcpy(secs,"0");
				sprintf(temp, "%d", sec);
				strcat(secs,temp);	//create seconds string
			}
			else
				sprintf(secs, "%d", sec);

			if(uid_num == uid && gid_num == gid)	//if uid of process matches present user then print
			{
				printf("%6s %2s:%2s:%2s %s\n", pid_str,hrs,mins,secs,name);
			}
		}
	}
}
