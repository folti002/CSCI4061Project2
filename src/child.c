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
	struct stat* st = (struct stat*) malloc(sizeof(struct stat));

	dir = opendir(name);
	if(dir == NULL){
		fprintf(stderr, "ERROR: Failed to open path: %s\n", name);
	}
	
	//Recursively traverse the directory and call SearchForPattern when neccessary
	while((entry = readdir(dir)) != NULL){
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
		char entry_name[MAX_PATH_LENGTH] = {'\0'};
		snprintf(entry_name, sizeof(entry_name), "%s/%s", name, entry->d_name);
		stat(entry_name, st);

		if(S_ISDIR(st->st_mode)){
			dirTraverse(entry_name, pattern);
		} else if(S_ISREG(st->st_mode)){
			searchPatternInFile(entry_name, pattern);
		} else {
			// This is neither a regular file nor a directory, so for now print error
			printf("This file is neither a regular file nor a directory.\n");
			exit(EXIT_FAILURE);
		}
	}
	free(st);
}

int main(int argc, char** argv){

	if(argc !=3){
	
		fprintf(stderr,"Child process : %d recieved %d arguments, expected 3 \n",getpid(), argc);
		fprintf(stderr,"Usage child.o [Directory Path] [Pattern] \n");
		exit(EXIT_FAILURE);
	}
	
	char* path = argv[1];
	char* pattern = argv[2];

	printf("Child with path: %s and pattern: %s\n", path, pattern);
	
	close(STDIN_FILENO);
	
	dirTraverse(path,pattern);
	
	close(STDOUT_FILENO);
	
	exit(EXIT_SUCCESS);
}
