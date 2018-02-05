#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char **argv)
{
	char path[100];	//store path name
	size_t size=100;	//size of storage buffer

	getcwd(path,size);	//system call to get present directory
	printf("%s\n",path);	//print to terminal


}
