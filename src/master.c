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
	
	// Declare variables to be used
	DIR* rootDir;
	struct dirent* entry;
	struct stat* st = (struct stat*) malloc(sizeof(struct stat));
	int numSubDir = 0;
	pid_t pid;

	// Open root directory
	rootDir = opendir(path);
	if(rootDir == NULL){
		fprintf(stderr, "ERROR: Failed to open path: %s\n", path);
		exit(EXIT_FAILURE);
	}
	
	// Iterate through root dir and spawn children as neccessary
	while((entry = readdir(rootDir)) != NULL){
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
		//Process each file/directory in the root dir
		char entry_name[MAX_PATH_LENGTH] = {'\0'};
		snprintf(entry_name, sizeof(entry_name), "%s/%s", path, entry->d_name);
		stat(entry_name, st);
		//printf("%s\n", entry_name);

		// Create a child if current entry is a directory
		if(S_ISDIR(st->st_mode)){
			numSubDir++;

			// Set up pipe
			int fds[2];
			int pipe_result = pipe(fds);
			if(pipe_result < 0){
				fprintf(stderr, "ERROR: Failed to open pipe.\n");
				exit(EXIT_FAILURE);
			}

			// Create a new child process
			pid = fork();

			// Check which process we are in
			if(pid < 0){
				// Error
				fprintf(stderr, "ERROR: fork failed.\n");
			} else if(pid > 0){
				// Parent process, which will read from the pipe

				// Close write end of the pipe
				close(fds[1]);

			} else {
				// Child process, which will write to the pipe

				// Close read end of the pipe
				close(fds[0]);
				// Do I/o redirection for child
				// Execute the child program so it can start 
				execl("child", "child", entry_name, pattern, NULL);
			}
		} else if(S_ISREG(st->st_mode)){
			// This is a regular file, so master process searches this on its own
		} else {
			// This is neither a regular file nor a directory, so for now print error
			printf("This file is neither a regular file nor a directory.\n");
			exit(EXIT_FAILURE);
		}
	}	

	// Free malloc'd variable for holding stats of a file	
	free(st);

	//Wait for all children to complete
	pid_t terminated_pid;
	for(int i = 0; i < numSubDir; i++){
		terminated_pid = wait(NULL);
	}
	
	// Read pipes of all children and print to stdout
	// Assumption : Pipe never gets full
	
	return 0;
}
