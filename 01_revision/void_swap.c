//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #2
// FILE PURPOSE:
// Swaping two variables with pointers.
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void *a, void *b, size_t size) {
  	char temp[size];
  	memcpy(temp, b, size);
  	memcpy(b, a, size);
  	memcpy(a, temp, size);
}

int main(){
	char a[50], b[50];
	fgets(a, 50, stdin);
	fgets(b, 50, stdin);
	swap(&a, &b, sizeof a);
	printf("%s%s", a, b);
	return 0;
}
