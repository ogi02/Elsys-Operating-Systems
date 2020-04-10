//--------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: shell.c
// FILE PURPOSE:
// Implementation of command shell in C.
//---------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 100


//--------------------------------------------
// FUNCTION: count_words()
// Count the words in the command line
// PARAMETERS:
// cmdline (type: const char*) - the command line
//----------------------------------------------
int count_words(const char* cmdline) {
	int count = 0;
	for(int i = 0; i < strlen(cmdline); i++) {
		if(cmdline[i] == ' ') {
			count ++;
		}
	}
	// 3 spaces -> 4 words
	// one for NULL
	return count + 2;
}

//--------------------------------------------
// FUNCTION: parse_cmdline()
// Split the command and put it into array of words
// PARAMETERS:
// cmdline (type: const char*) - the command line
//----------------------------------------------
char** parse_cmdline(const char* cmdline) {

	// copy of the command line, so it is not formatted
	char *command = cmdline;

	// index of arguments read
	int index = 0;

	// split for strtok() function
	char split[2] = " ";

	// memory allocation
	char **arguments;
	arguments = malloc(count_words(command) * sizeof(char *));
   
   	// splitting the command line into separate arguments
	char *token = strtok(command, split);   
	while(token != NULL){
		arguments[index] = token;
		index ++;
		token = strtok(NULL, split); 
	}
	arguments[index] = NULL;
	
	return arguments;
	
}

int main() {
	
	// buffer for command
	char buffer[BUFFER_SIZE];

	while(1){

		// task requirement
		printf("$ ");
		
		// read command
		if(fgets(buffer, BUFFER_SIZE - 1, stdin) == NULL){
			break;
		}

		buffer[strlen(buffer) - 1] = '\0';

		// parse command line
		char **args = parse_cmdline(buffer);

		// new process
		pid_t pid = fork();

		int exe_value, wait_value;
		
		if(pid < 0){
			// error
			perror("fork");
		}
		// child process
		else if(pid == 0){

			// execute 
			exe_value = execvp(args[0], args);
			if(exe_value < 0){
				// error
				perror(args[0]);
			}

		}
		// parent process
		else {
			// wait for child process to complete
			if(waitpid(pid, &wait_value, 0) < 0){
				// error
				perror("waitpid");
			}
		}

		// memory freeing
		free(args);

		// if in child process, a.k.a if error occured
		if(pid == 0) {
			return 1;
		}
	}

	return 0;
}
