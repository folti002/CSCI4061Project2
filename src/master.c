#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"



int main(int argc, char** argv){

	if(argc != 3){
		fprintf(stderr,"Usage ./a.out [Path to Directory] [Pattern to search] \n");
		exit(EXIT_FAILURE);
	}
	char* path = argv[1];
	char* pattern = argv[2];
	printf("Path: %s\nPattern: %s\n", path, pattern);
	
	// Declare variables to be used
	DIR* rootDir;
	struct dirent* entry;
	struct stat* st = (struct stat*) malloc(sizeof(struct stat));
	int numSubDir = 0;
	pid_t pid;

	// Open root directory
	rootDir = opendir(path);
	if(rootDir == NULL){
		printf("Failed to open path: %s\n", path);
	}
	
	// Iterate through root dir and spawn children as neccessary
	while((entry = readdir(dir)) != NULL){
		//Process each file/directory in the root dir
		//create a child if the file is directory
			//Create a pipe
			//Do I/o redirection for child
			//Exec child

		char entry_name[100] = {'\0'};
		snprintf(entry_name, sizeof(entry_name), "%s%s", path, entry->d_name);

		//stat(entry_name, st);
		printf("%s\n", entry_name);
	}	
	
	free(st);

	//Wait for all children to complete

	
	
	// Read pipes of all children and print to stdout
	// Assumption : Pipe never gets full
	
	return 0;
}
