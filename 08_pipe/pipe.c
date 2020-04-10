#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {

	// argument for pipe
	int fd[2];

	// variable for fork
	pid_t pid;

	if(pipe(fd) < 0) {
		// error
		perror("pipe");
		return 1;
	}

	pid = fork();
	// error
	if(pid < 0) {
		perror("fork");
		return 1;
	}
	// child process
	else if(pid == 0) {
		// closing reading part of pipe because we are not using it
        if(close(fd[0]) < 0) {
        	perror("close");
        	return 1;
        } 

		// closing stdout so that we cannot write on stdout
		if(close(fileno(stdout)) < 0) {
			perror("close");
			return 1;
		} 
          
        // copying fd[1] to the standard output so that "ls" can write its returned value to fd[1]
        if(dup(fd[1]) < 0) {
        	perror("dup");
        	return 1;
        } 
          
        // execute "ls"
        if(execlp("ls", "ls", "-l", NULL) < 0) {
        	perror("execlp");
        	return 1;
        } 
	}
	// parent process
	else {
		// closing writing part of pipe because we are not using it
        if(close(fd[1]) < 0) {
        	// error
        	perror("close");
        	return 1;
        }

		// closing stdin so that we cannot read from stdin 
        if(close(fileno(stdin)) < 0) {
        	perror("close");
        	return 1;
        } 
          
        // copying fd[0] to the standard input so that "wc" can read what is in fd[0] 
        if(dup(fd[0]) < 0) {
        	perror("dup");
        	return 1;
        } 

        // execute "wc"  
        if(execlp("wc", "wc", "-l", NULL) < 0) {
        	perror("execlp");
        	return 1;
        } 
	}

	if(close(pid) < 0) {
		// error
		perror("close");
		return 1;
	}

	return 0;
}
