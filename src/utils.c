#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"


/*
	Searches for pattern in file and writes the entire line containing the pattern to STDOUT
	Format for print: [path to file] : Line in the file
	@Params:
		path - path to the file
		pattern : pattern to be searched
*/
void searchPatternInFile(char* path, char* pattern){
	FILE * fp;
	fp = fopen(path, "r");
	if (fp == NULL){
		fprintf(stderr, "Error opening file: %s\n", path);
		return;
	}

	// Read file line by line and check if pattern exists in the line
	char line[MAX_LINE_LENGTH];
	memset(line, '\0', MAX_LINE_LENGTH);
	while((fgets(line, MAX_LINE_LENGTH, fp)) != NULL){
		char* phrase = strstr(line, pattern);
		if(phrase != 0){
			char* pathPhrase = (char*) malloc(sizeof(char) * MAX_LINE_LENGTH);
			sprintf(pathPhrase, "%s: %s", path, line);
			write(STDOUT_FILENO, pathPhrase, MAX_LINE_LENGTH);
			free(pathPhrase);
		}
	}
	fclose(fp);
}