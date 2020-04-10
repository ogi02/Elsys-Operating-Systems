#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 100

int main(){
	int fd = open("a.txt", O_RDONLY);
	if(fd == -1){
		perror("open a.txt");
		return 1;
	}

	char buffer[BUFFER_SIZE];
	ssize_t read_value, write_value;
	
	read_value = read(fd, buffer, BUFFER_SIZE);
	if(read_value == -1){
		perror("read");
		return 1;
	}
	printf("%ld\n", read_value);

	write_value = write(STDOUT_FILENO, buffer, read_value);
	if(write_value == -1){
		perror("write");
		return 1;
	}

	int close_return_value = close(fd);
	if(close_return_value == -1){
		perror("close");
		return 1;
	}

	return 0;
}
