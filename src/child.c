/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"


/*
	Traverse the directory recursively and search for pattern in files.
	@params:
		name - path to the directory
		pattern - pattern to be recusrively searched in the directory
		
	Note: Feel free to modify the function header if neccessary
	
*/
void dirTraverse(const char *name, char * pattern){
	DIR *dir;
	struct dirent *entry;

	// Open directory
	dir = opendir(name);
	if(dir == NULL){
		fprintf(stderr, "ERROR: Failed to open path: %s\n", name);
	}
	
	// Recursively traverse the directory and call searchPatternInFile when neccessary
	while((entry = readdir(dir)) != NULL){
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;

		char entry_name[MAX_PATH_LENGTH] = {'\0'};
		snprintf(entry_name, sizeof(entry_name), "%s/%s", name, entry->d_name);

		// Check which kind of file we are currently looking at
		if(entry->d_type == DT_DIR){
			dirTraverse(entry_name, pattern);
		} else if(entry->d_type == DT_REG){
			searchPatternInFile(entry_name, pattern);
		} else {
			// This is neither a regular file nor a directory, so for now print error
			printf("This file is neither a regular file nor a directory.\n");
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char** argv){
	if(argc !=3){
		fprintf(stderr,"Child process : %d recieved %d arguments, expected 3 \n", getpid(), argc);
		fprintf(stderr,"Usage child.o [Directory Path] [Pattern] \n");
		exit(EXIT_FAILURE);
	}
	
	char* path = argv[1];
	char* pattern = argv[2];

	// Close file descriptors and traverse the given directory
	close(STDIN_FILENO);
	dirTraverse(path, pattern);
	close(STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}
