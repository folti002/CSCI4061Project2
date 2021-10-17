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
	
	//Declare any other neccessary variables
		
	
	//Open root directory
	
	
	// Iterate through root dir and spawn children as neccessary
	
	
		//Process each file/directory in the root dir
		//create a child if the file is directory
			//Create a pipe
			//Do I/o redirection for child
			//Exec child
		
	
	
	
	//Wait for all children to complete
	
	
	//Read pipes of all children and print to stdout
	//Assumption : Pipe never gets full
	
	return 0;
}
