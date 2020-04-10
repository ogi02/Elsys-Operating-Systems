//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: lseek-reverse.c
// FILE PURPOSE:
// Algorithm for creating secret messages,
// contains blocks with data and the address of the next element
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct {
	char data;
	unsigned char nextElementAddress;
} block;

//------------------------------------------------------------------------
// FUNCTION: generateFile
// Opens the hidden file and writes the secret message using the order of the block from generateOrder()
// PARAMETERS:
// order (type: int*) - order of the blocks
// msg (type: char*) - the message to be encrypted
// filename (type: char*) - the file which contains the encrypted message
//------------------------------------------------------------------------

void generateFile(int *order, char *msg, char *filename){
	
	// open file (write only, create if it doesn't exist, clear everything in it if it exists)
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
	if(fd < 0){
		perror("read");
		return;
	}

	block file_blocks[128];

	// set default values for the blocks
	for(int i = 0; i < 128; i++){
		file_blocks[i].data = 0;
		file_blocks[i].nextElementAddress = 0;
	}

	// set real values to the blocks, using the order and the message
	for(int i = 0; i < strlen(msg); i++){
		if(i == 0){
			file_blocks[0].data = *(msg + 0);
			file_blocks[0].nextElementAddress = *(order + 0) * 2;
		}
		else{
			file_blocks[order[i - 1]].data = *(msg + i);
			file_blocks[order[i - 1]].nextElementAddress = *(order + i) * 2;
		}
	}

	// write all the blocks
	int write_value = write(fd, file_blocks, 255);
	if(write_value < 0){
		perror("write");
		return;
	}

	// close file
	int close_value = close(fd);
	if(close_value < 0){
		perror("close");
		return;
	}

}

//------------------------------------------------------------------------
// FUNCTION: generateOrder
// Generates the order of the blocks in the hidden file.
// PARAMETERS:
// length (type: int) - length of message, consequently size of the array with the order
//------------------------------------------------------------------------

int* generateOrder(int length){
	int random;
	
	// array which contains the order of the blocks
	static int *order;
	order = malloc(length * sizeof(int));
	srand(time(NULL));

	// array with all empty blocks (with indexes)
	int empty[126];
	int count = 126;
	for(int i = 1; i < 127; i++){
		empty[i - 1] = i;
	}

	// pick one of the empty blocks and fill it with data and the address of the next element
	for(int i = 0; i < length; i++){
		random = rand() % count;
		order[i] = empty[random];

		// remove the index of the newly filled block from the array with empty blocks 
		for(int j = random; j < count - 1; j++)
        	empty[j] = empty[j + 1];
        count --;
	}
	
	return order;
}

int main(int argc, char *argv[]){

	char *filename = argv[1];

	// read message
	char msg[127];
	int read_value = read(STDIN_FILENO, msg, 127);
	if(read_value < 0){
		perror("read");
		return 1;
	}
	msg[read_value] = '\0';

	int size = strlen(msg);

	// generate order
	int *order = generateOrder(size);
	*(order + size - 1) = 0;

	// generate file
	generateFile(order, msg, filename);

	// free dynamic memory
	free(order);

	return 0;
}
