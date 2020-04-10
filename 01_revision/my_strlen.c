//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE PURPOSE:
// Own implementation of 'strlen' function in C.
//------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>

int strlen(char* str){
	int i;
	for(i = 0; str[i] != '\0'; i++);
	return i;
}

int main(){
	char str[250];
	fgets(str, 250, stdin);
	printf("%d", strlen(str));
}
