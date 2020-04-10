//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: tail-management.h
// FILE PURPOSE:
// Helpful functions regarding the implementation of tail in C
// Header design and Error message generation
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// FUNCTION: generateHeader()
// Generates headers when there are two or more files as arguments
// PARAMETERS:
// filename (type: char*) - the name of the current file, used in the header
// file (type: int) - number of files that have been processed
//------------------------------------------------------------------------

char* generateHeader(char* filename, int files){
	static char header[1000];
	strcpy(header, (files > 1) ? "\n==> " : "==> ");
	strcat(header, filename);
	strcat(header, " <==\n");
	return header;
}


//------------------------------------------------------------------------
// FUNCTION: generateOpenErrorMessage
// Generates the message when there is an error returned from open().
// PARAMETERS:
// filename (type: char*) - name of the file, used in the error message
//------------------------------------------------------------------------

char* generateOpenErrorMessage(char *filename){
	static char error_message[1000];
	strcpy(error_message, "tail: cannot open \'");
	strcat(error_message, filename);
	strcat(error_message, "\' for reading");
	return error_message;
}


//------------------------------------------------------------------------
// FUNCTION: generateReadErrorMessage
// Generates the message when there is an error returned from read().
// PARAMETERS:
// filename (type: char*) - name of the file, used in the error message
//------------------------------------------------------------------------

char* generateReadErrorMessage(char *filename){
	static char error_message[1000];
	strcpy(error_message, "tail: error reading \'");
	strcat(error_message, filename);
	strcat(error_message, "\'");
	return error_message;
}


//------------------------------------------------------------------------
// FUNCTION: generateWriteErrorMessage
// Generates the message when there is an error returned from write().
// PARAMETERS:
// none
//------------------------------------------------------------------------

char* generateWriteErrorMessage(){
	static char error_message[1000];
	strcpy(error_message, "tail: error writing \'standard output\'");
	return error_message;
}


//------------------------------------------------------------------------
// FUNCTION: generateCloseErrorMessage
// Generates the message when there is an error returned from close().
// PARAMETERS:
// filename (type: char*) - name of the file, used in the error message
//------------------------------------------------------------------------

char* generateCloseErrorMessage(char* filename){
	static char error_message[1000];
	strcpy(error_message, "tail: error reading \'");
	strcat(error_message, filename);
	strcat(error_message, "\'");
	return error_message;
}
