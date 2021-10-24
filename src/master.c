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
	int fds[2];

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

		// Create a child if current entry is a directory
		if(S_ISDIR(st->st_mode)){
			numSubDir++;

			// Set up pipe
			int pipe_result = pipe(fds);
			if(pipe_result < 0){
				fprintf(stderr, "ERROR: Failed to open pipe.\n");
				exit(EXIT_FAILURE);
			}

			// Create a new child process and check which process we are in
			pid = fork();
			if(pid < 0){
				// Error
				fprintf(stderr, "ERROR: fork failed.\n");
			} else if(pid > 0){
				// Parent process, which will read from the pipe
				// Close write end of the pipe
				close(fds[1]);

				// Variable to store the read line from a file
				char* line = (char*) malloc(sizeof(char) * MAX_LINE_LENGTH);
				memset(line, '\0', (sizeof(char) * MAX_LINE_LENGTH));

				// Read pipes of all children and print to stdout
				// Assumption : Pipe never gets full
				while(read(fds[0], line, MAX_LINE_LENGTH)){
					printf("%s", line);
				}
				close(fds[0]);
				free(line);
			} else {
				// Child process, which will write to the pipe
				// Close read end of the pipe
				close(fds[0]);

				// I/O redirection for child
				// STDOUT has been redirected to the write end of the pipe
				if(dup2(fds[1], STDOUT_FILENO) == -1){
					fprintf(stderr, "ERROR: Failed to dup2 STDOUT\n");
					exit(EXIT_FAILURE);
				}
				close(fds[1]);

				// Execute the child program so it can start reading from files
				execl("child", "child", entry_name, pattern, NULL);
				fprintf(stderr, "ERROR: Failed to execute child\n");
				exit(EXIT_FAILURE);
			}
		} else if(S_ISREG(st->st_mode)){
			// This is a regular file, so master process searches this on its own
			FILE* fp2 = fopen(entry_name, "r");
			if(fp2 == NULL){
				fprintf(stderr, "ERROR: Failed to open file: %s\n", entry_name);
				exit(EXIT_FAILURE);
			}

			// Read through regular file and print any matches
			char line[MAX_LINE_LENGTH];
			memset(line, '\0', MAX_LINE_LENGTH);
			while((fgets(line, MAX_LINE_LENGTH, fp2)) != NULL){
				char* phrase = strstr(line, pattern);
				if(phrase != 0){
				  printf("%s: %s", entry_name, line);
				}
			}
			fclose(fp2);
		} else {
			// This is neither a regular file nor a directory, so for now print error
			printf("This file is neither a regular file nor a directory.\n");
			exit(EXIT_FAILURE);
		}
	}	

	// Free malloc'd variable for holding stats of a file	
	free(st);

	// Wait for all children to complete
	pid_t terminated_pid;
	for(int i = 0; i < numSubDir; i++){
		terminated_pid = wait(NULL);
		if(terminated_pid < 0){
			fprintf(stderr, "ERROR: Failed to wait properly\n");
			exit(EXIT_FAILURE);
		}
	}
	return EXIT_SUCCESS;
}