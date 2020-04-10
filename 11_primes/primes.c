#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

// checks if a number is prime
int is_prime(int num) {
	
	// loops through possible delimeters
	for(int i = 2; i < sqrt(num) + 1; i++) {
		// number is not prime
		if(num % i == 0) {
			return 0;
		}
	}

	// number is prime
	return 1;
}

// loops through all numbers from 2 to N
void *work(void *arg){
	
	// take argument
	int num = *((int *) arg);
	free(arg);

	printf("Prime calculation started for N=%d\n", num);

	// count for the numbers
	int count = 0;

	// no prime numbers in this range
	if(num <= 2) {
		
	}
	else {
		for(int i = 2; i < num; i++) {
			if(is_prime(i)) {
				count ++;
			}
		}
		count ++;
	}

	printf("Number of primes for N=%d is %d\n", num, count);
}

int main() {
	pthread_t threads[100];
	int count = 0;

	char input[100];

	while(1) {
		// check for "e"
		if(strcmp(fgets(input, 250, stdin), "e\n") == 0) {
			for(int i = 0; i < count; i++) {
				pthread_join(threads[i], NULL);
			}
			exit(0);
		}

		// get number from input
		int num = 0;
		if(input[0] == 'p') {
			for(int i = 2; input[i] != '\n'; i++) {
				num *= 10;
				num += input[i] - '0';
			}
		}

		// create thread
		int *p = malloc(sizeof(int));
		*p = num;
		pthread_create(&threads[count ++], NULL, work, p);
	}

	return 0;
}
