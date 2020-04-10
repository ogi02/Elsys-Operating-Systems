//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #2
// FILE PURPOSE:
// Duplicate string using dynamic memory.
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strdup(const char *str){
	char *duplicate = malloc(sizeof(strlen(str) + 1));
	strcpy(duplicate, str);
	return duplicate;
}

int main(){
	char str[250];
	fgets(str, 250, stdin);
	printf("%s", strdup(str));
	return 0;
}
