//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE PURPOSE:
// Reading from .png files and extracting width and height in C.
//------------------------------------------------------------------------


#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <arpa/inet.h>
#include <string.h>

void *read_image( const char *filepath );

int main( int argc, const char* argv[] ) {

	char *ext = strrchr(argv[1], '.');
	if(!ext){ 
    	printf("Unable to open file\n");
	} 
	else if(strcmp(ext + 1, "png") != 0){ 
     	printf("Unable to parse file\n");
	}
	else{
		char *p = read_image(argv[1]);
		if(p == NULL){
			printf("Unable to open file\n");
			return 0;
		}

		uint32_t width = ntohl(*(uint32_t*)&p[16]);    
 		uint32_t height = ntohl(*(uint32_t*)&p[20]);

		printf("PNG image width: %u\n", width);
		printf("PNG image height: %u\n", height);

		uint32_t size = 8;
		int chunk_size;

		while(1){
			chunk_size = ntohl(*(uint32_t*)&p[size]);
			if(p[size + 4] == 0x49 && p[size + 5] == 0x45 && p[size + 6] == 0x4E && p[size + 7] == 0x44){
				size += 12; // chunk size info, type, CRC
				size += chunk_size;
				break;
			}
			else{
				size += 12; // chunk size info, type, CRC
				size += chunk_size;
			}
		}

		printf("PNG file size: %u\n", size);
	}
}
