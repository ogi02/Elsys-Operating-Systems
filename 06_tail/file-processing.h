//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: file-processing.h
// FILE PURPOSE:
// Operates with file:
// • opening and closing
// • reading from the back and locating the beginning of the last 10 lines
// • reading and writing the last 10 lines in the standard output
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// FUNCTION: processFile()
// • open and close the file
// • read from the back and locate the beginning of the last 10 lines
// • read and write the last 10 lines in the standard output
// PARAMETERS:
// filename (type: char*) - name of the file to be processed
// all_files (type: int) - argument count; used for designing headers
// done_files (type: int) - all files that have passed without error; used for designing header
//------------------------------------------------------------------------

int processFile(char *filename, int all_files, int done_files){

	// returned values for read(), write(), lseek(), close()
	ssize_t read_value, write_value, lseek_value, close_value;

	// lines read
	int lines = 0;

	// open and check for possible error
	int fd = open(filename, O_RDONLY);
	if(fd < 0){
		perror(generateOpenErrorMessage(filename));
		return 1;
	}

	// check file size, used if offset will go outside the file
	lseek_value = lseek(fd, 0, SEEK_END);
	if(lseek_value < 0){
		perror("lseek");
		return 1;
	}

	// file size (bytes)
	int file_size = lseek_value;

	// used to find size of last 10 lines
	int symbols_read = 0;

	// buffer character
	char ch;

	do {

		// lseek and check for possible error
		lseek_value = lseek(fd, --symbols_read, SEEK_END);
		if(lseek_value < 0){
			perror("lseek");
			return 1;
		}

		// read and check for possible error
		read_value = read(fd, &ch, 1);
		if(read_value < 0){
			perror(generateReadErrorMessage(filename));
			return 1;
		}

		// if there is a new line
		if(ch == '\n' && symbols_read != 0){
			lines ++;
        }

	} while (lines <= LINES_TO_READ && lseek_value > 0);

	// bytes of last 10 lines
	int bytes_to_read = symbols_read * (-1) - 1;

	// didn't work otherwise ;)
	if(bytes_to_read == file_size - 1){
		lseek_value = lseek(fd, 0, SEEK_SET);
		if(lseek_value < 0){
			perror("lseek");
			return 1;
		}
		bytes_to_read ++;
	}

	// write headers
	if(all_files > 2){
		char* header = generateHeader(filename, done_files);
		write_value = write(STDOUT_FILENO, header, strlen(header));
		if(write_value < 0){
			perror(generateWriteErrorMessage());
			return 1;
		}
	}

	while(bytes_to_read > 0){

		do {
			// read and check for possible error
			read_value = read(fd, &ch, 1);
			if(read_value < 0){
				perror(generateReadErrorMessage(filename));
				return 1;
			}
		} while (read_value == 0);

		do {
			// write and check for possible error
			write_value = write(STDOUT_FILENO, &ch, 1);
			if(write_value < 0){
				perror(generateWriteErrorMessage());
				return 1;
			}
		} while (write_value == 0);

		bytes_to_read --;
	}
	
	// close and check for possible error
	close_value = close(fd);
	if(close_value < 0){
		perror(generateCloseErrorMessage(filename));
		return 1;
	}

	return 0;
}
