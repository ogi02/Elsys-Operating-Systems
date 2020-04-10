//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: watch.c
// FILE PURPOSE:
// Implement system function 'watch' in C.
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SLEEP_TIME 2

int main(int argc, char *argv[]) {
	
	// counter
	int i;

	// process ID
	pid_t pid;	

	// array of arguments to call 'execvp' with
	char *arguments[50] = {};
    char **ptr = arguments;

    // put argument values in variable of type char** 
    for(i = 0; i < argc - 1; i++){
    	ptr[i] = argv[i + 1];
    }
    ptr[i] = NULL;

    // execvp() return value and waitpid() return value
	int exe_value, wait_value;

	while(1){

		sleep(SLEEP_TIME);
		
		// new process
		pid = fork();
		
		if(pid < 0){
			// error
			perror("fork");
		}
		
		else if(pid == 0){
			// call execute			
			exe_value = execvp(argv[1], arguments);
			if(exe_value < 0){
				// error
				perror(argv[1]);
				return 1;
			}
		}
		
		else {
			// wait for child process to complete
			waitpid(pid, &wait_value, 0);
		}
	}
	
	return 0;
}