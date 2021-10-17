#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include<string.h>
#include<stdlib.h>
#include "utils.h"


/*
	Traverse the directory recursively and search for pattern in files.
	@params:
		name - path to the directory
		pattern - pattern to be recusrively searched in the directory
		
	Note: Feel free to modify the function header if neccessary
	
*/
void dirTraverse(const char *name, char * pattern)
{
	DIR *dir;
	struct dirent *entry;
	
	//Recursively traverse the directory and call SearchForPattern when neccessary
}

int main(int argc, char** argv){

	if(argc !=3){
	
		fprintf(stderr,"Child process : %d recieved %d arguments, expected 3 \n",getpid(), argc);
		fprintf(stderr,"Usage child.o [Directory Path] [Pattern] \n");
		exit(EXIT_FAILURE);
	}
	
	char* path = argv[1];
	char* pattern = argv[2];
	
	close(STDIN_FILENO);
	
	dirTraverse(path,pattern);
	
	close(STDOUT_FILENO);
	
	exit(EXIT_SUCCESS);
}
