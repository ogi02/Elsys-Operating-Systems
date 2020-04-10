//--------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE PURPOSE: 
// Task with cars and drivers, using trylock for testing the cars.
//---------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define DRIVERS 20
#define CARS 5

pthread_mutex_t cars[CARS];

void* drive(void* arg) {

	int driver_id = *((int *) arg);
	free(arg);

	int car_tested[CARS];
	for(int i = 0; i < CARS; i++) {
		car_tested[i] = 0;
	}

	int cars_left_to_test = 5;

	while(1) {
		for(int i = 0; i < CARS; i++) {
			if(car_tested[i] == 0) {
				if(pthread_mutex_trylock(&cars[i]) == 0) {
					// print task requirements
					printf("Buyer %d takes car %d.\n", driver_id, i + 1);
					printf("Buyer %d returns car %d.\n", driver_id, i + 1);
					car_tested[i] = 1;
					cars_left_to_test --;
					pthread_mutex_unlock(&cars[i]);
				}
			}
		}
		if(cars_left_to_test == 0) {
			break;
		}
	}

	return NULL;

}

int main() {
	
	// init cars' mutex
	for(int i = 0; i < CARS; i++) {
		pthread_mutex_init(&cars[i], NULL);
	}

	pthread_t drivers[DRIVERS];

	// create drivers' threads
	for(int i = 0; i < DRIVERS; i++) {
		int *p = malloc(sizeof(int));
		*p = i + 1;
		pthread_create(&drivers[i], NULL, drive, p);
	}

	for(int i = 0; i < DRIVERS; i++) {
		pthread_join(drivers[i], NULL);
	}

	// destroy cars' mutex
	for(int i = 0; i < CARS; i++) {
		pthread_mutex_destroy(&cars[i]);
	}

	return 0;
}