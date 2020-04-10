//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: stdout-processing.h
// FILE PURPOSE:
// Operates with standard output:
// • reading from standard input and writing in a separate file
// • call processFile() from file-processing.h
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// FUNCTION: processStdout()
// • reading from standard input and writing in a separate file
// • call processFile() from file-processing.h
// PARAMETERS:
// number_of_user_inputs (type: int) - indicates when to execute tail of user's input
//------------------------------------------------------------------------

void processStdout(){
	
	// returned values for read(), write(), lseek(), close()
	ssize_t read_value, write_value, lseek_value, close_value;

	// name of the file where the user's input is stored
	char filename[250] = { "stdout-processing.txt" };

	// open and check for possible error
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
	if(fd < -1){
		perror(generateOpenErrorMessage(filename));
		return;
	}

	lseek_value = lseek(fd, 0, SEEK_END);
	if(lseek_value < 0){
		perror("lseek");
		return;
	}

	// buffer for reading
	char buffer;

	while(1){

		// read and check for possible error
		read_value = read(STDIN_FILENO, &buffer, 1);
		if(read_value < 0){
			perror("read");
			return;
		}

		// if ctrl+d is pressed
		if(read_value == 0){
			break;
		}

		// write and check for possible error
		write_value = write(fd, &buffer, 1);
		if(write_value < 0){
			perror("write");
			return;
		}

	}

	// close and check for possible error
	close_value = close(fd);
	if(close_value < 0){
		perror(generateCloseErrorMessage(filename));
		return;
	}

	processFile(filename, 0, 0);

}
