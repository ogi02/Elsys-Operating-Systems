//--------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: writer.c
// FILE PURPOSE:
// Write part of "named pipe".
//---------------------------------------------

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

int main() {
	char *myfifo = "../myfifo";
	char buffer[BUFFER_SIZE];
	
	// remove named pipe if it already exists
	unlink(myfifo);

	// create the named pipe
	mkfifo(myfifo, 0666);

	// open and check for possible error
	int fd = open(myfifo, O_WRONLY | O_CREAT);
	if(fd < 0) {
		perror("open");
		return 1;
	}

	while(1) {
		
		fgets(buffer, BUFFER_SIZE, stdin);

		ssize_t write_value = write(fd, buffer, strlen(buffer));
		if(write_value < 0) {
			perror("write");
			return 1;
		} 
	
	}

	
	ssize_t close_value = close(fd);
	if(close_value < 0) {
		perror("close");
		return 1;
	}

	// remove the named pipe
	unlink(myfifo);

	return 0;
}
