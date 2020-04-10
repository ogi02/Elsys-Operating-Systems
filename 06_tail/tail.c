//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: tail.c
// FILE PURPOSE:
// • all defines and includes
// • handling arguments given to the main
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define LINES_TO_READ 10
#define BUFFER_SIZE 10000000

#include "tail-management.h"
#include "file-processing.h"
#include "stdout-processing.h"

int main(int argc, char *argv[]) {

	int done_files = 1;
	int error;

	for(int files = 1; files < argc; files++){
		
		// user input
		if(argv[files][0] == '-'){
			processStdout();
		}

		// file input
		else {
			error = processFile(argv[files], argc, done_files);
			if(error == 0){
				done_files ++;
			}
		}
	}

	// user input
	if(argc == 1){
		processStdout();
	}

	return 0;
}
