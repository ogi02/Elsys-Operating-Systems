//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #2
// FILE PURPOSE:
// Own implementation of 'strcat' function in C.
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

int strlen(char* str){
	int i;
	for(i = 0; str[i] != '\0'; i++);
	return i;
}

char* strcat(char* destination, const char* source){
    int c = strlen(destination);
    for(int i = 0; source[i] != '\0'; i++){
        destination[c]=source[i];
        c++;
    }
    destination[c+1]='\0';
    return destination;
}

int main(){
	char destination[250], source[250];
	fgets(destination, 250, stdin);
	fgets(source, 250, stdin);
	printf("%s", strcat(destination, source));
	return 0;
}
