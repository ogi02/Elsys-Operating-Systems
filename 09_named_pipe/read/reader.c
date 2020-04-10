//--------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: reader.c
// FILE PURPOSE:
// Read part of "named pipe".
//---------------------------------------------

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

int main() {
	char *myfifo = "../myfifo";
	char buffer[BUFFER_SIZE];	

	// open and check for possible error
	int fd = open(myfifo, O_RDONLY | O_CREAT);
	if(fd < 0) {
		perror("open");
		return 1;
	}

	while(1) {

		// read from "myfifo" and check for possible error
		ssize_t read_value = read(fd, buffer, BUFFER_SIZE);
		if(read_value < 0) {
			perror("read");
			return 1;
		}
		
		// if it has read anything
		if(read_value > 0) {
			// remove new line
			buffer[strlen(buffer) - 1] = '\0';

			// print received message 
			printf("Message: %s\n", buffer);
			for(int i = 0; i < strlen(buffer); i++) {
				buffer[i] = '\0';
			}
		}

	}
	
	// close and check for possible error
	ssize_t close_value = close(fd);
	if(close_value < 0) {
		perror("close");
		return 1;
	}

	return 0;
}
