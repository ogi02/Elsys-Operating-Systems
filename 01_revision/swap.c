//------------------------------------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #2
// FILE PURPOSE:
// Swaping two integers with pointers.
//------------------------------------------------------------------------

#include <stdio.h>

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main(){
	int a, b;
	scanf("%d", &a);
	scanf("%d", &b);
	swap(&a, &b);
	printf("%d%d", a, b);
	return 0;
}
