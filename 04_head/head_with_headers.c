#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define READ_SIZE 1
#define BUFFER_SIZE 100000 
#define LINES_TO_READ 10

int main(int argc, char **argv){

	char filename[250];

	for(int i = 1; i < argc; i++){

		strcpy(filename, argv[i]);

		int fd = open(argv[i], O_RDONLY);
		if(fd == -1){
			perror("open");
			return 1;
		}
		
		if(argc > 2){
			strcpy(filename, (i > 1) ? "\n==> " : "==> ");
			strcat(filename, argv[i]);
			strcat(filename, " <==\n");
			write(STDOUT_FILENO, filename, strlen(filename));
		}

		int lines = 1, index = 0;
		ssize_t read_value, write_value;

		char ch, buffer[BUFFER_SIZE];

		do{	
			//printf("buffer -> %s\n", buffer);
			read_value = read(fd, &ch, READ_SIZE);
			if(read_value == -1){
				perror("read");
				return 1;
			}

			if(index == BUFFER_SIZE - 1){
				buffer[index] = ch;
				buffer[index + 1] = '\0';
				ch = 0;
				index = 0;
				write_value = write(STDOUT_FILENO, buffer, strlen(buffer));
		        if(write_value < 0){
		            perror("write");
		            return 1;
		        }
			}
			else if(ch == '\n'){

				buffer[index] = ch;
				buffer[index + 1] = '\0';
				ch = 0;
				index = 0;

				write_value = write(STDOUT_FILENO, buffer, strlen(buffer));
		        if(write_value < 0){
		            perror("write");
		            return 1;
		        }

		        if(lines == LINES_TO_READ){
		        	break;
	        	} 

		        lines ++;
	        }
	        else{
	        	buffer[index++] = ch;
	        }

		} while(read_value != 0);

		if(ch != 0){
			//printf("index -> %d\n", index);
			//printf("buffer -> %s\n", buffer);
			buffer[index - 1] = '\0';


			write_value = write(STDOUT_FILENO, buffer, strlen(buffer));
			if(write_value < 0){
	            perror("write");
	            return 1;
	        }

			write_value = write(STDOUT_FILENO, "\n", 1);
	        if(write_value < 0){
	            perror("write");
	            return 1;
	        }
		}

		int close_return_value = close(fd);
		if(close_return_value == -1){
			perror("close");
			return 1;
		}
	}

	return 0;
}
