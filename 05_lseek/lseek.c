//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: lseek.c
// FILE PURPOSE:
// Reading from file using lseek.
//------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]){
	
	// opening
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		perror("open");
		return 1;
	}

	int read_value, write_value, lseek_value;
	unsigned char buffer[2];

	do {

		// reading
		read_value = read(fd, buffer, 2);
		if(read_value < 0){
			perror("read");

		}

		// writing
		write_value = write(STDOUT_FILENO, buffer, 1);
		if(write_value < 0){
			perror("write");
			return 1;
		}

		// getting to next address
		lseek_value = lseek(fd, buffer[1], SEEK_SET);
		if(lseek_value < 0){
			perror("lseek");
			return 1;
		}

	} while(buffer[1] != 0);

	// closing
	int close_value = close(fd);
	if(close_value < 0){
		perror("close");
		return 1;
	}

	return 0;
}
