//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE PURPOSE:
// Reading from .bmp files and extracting width and height in C.
//------------------------------------------------------------------------

// Пращам задачата със закъснение, тъй като на 7 октомври не бях на училище и разбрах 
// впоследствие за задачите. Дано това не е проблем. Приятен ден.

#include <stdint.h>
#include <stdio.h>
#include <math.h>

void *read_image( const char *filepath );

int main( int argc, const char* argv[] ) {
	char *p = read_image(argv[1]);

	uint32_t width1 = *(uint32_t*)&p[18];    
 	uint32_t height1 = *(uint32_t*)&p[22];

	printf("BMP image width: %u\n", width1);
	printf("BMP image height: %u\n", height1);
}
