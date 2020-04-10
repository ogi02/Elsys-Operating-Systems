//--------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE PURPOSE: 
// Task with miners and traders using mutex.
//---------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MINED_GOLD 10
#define TRADED_GOLD 10
#define NUMBER_OF_ACTIONS 20
#define SLEEP_TIME 2

// gold count
unsigned int gold = 0;

// mutex used
pthread_mutex_t mutex;

// miner's action -> mining gold
void* mine(void* arg) {
	// get and free argument
	int id = *((int *)arg);
	free(arg);

	for(int i = 0; i < NUMBER_OF_ACTIONS; i++) {
		// lock mutex
		pthread_mutex_lock(&mutex);

		// mine gold
		gold += MINED_GOLD;
		printf("Miner %d gathered %d gold\n", id, MINED_GOLD);

		// unlock mutex
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

// trader's action -> trading gold
void* trade(void* arg) {
	// get and free argument
	int id = *((int *)arg);
	free(arg);

	for(int i = 0; i < NUMBER_OF_ACTIONS; i++) {
		// lock mutex
		pthread_mutex_lock(&mutex);

		// if there is not enough gold
		if(gold < TRADED_GOLD) {
			printf("The warehouse is empty, cannot sell!\n");
		}
		// trade gold
		else {
			gold -= TRADED_GOLD;
			printf("Trader %d sold %d gold\n", id, TRADED_GOLD);
		}

		// unlock mutex
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

int main(int argc, char const *argv[]) {
	// set default miner and trader count
	int count_of_miners = 1;
	int count_of_traders = 1;

	// use console arguments for miner and trader count
	if(argc > 3) {
		count_of_miners = atoi(argv[1]);
		count_of_traders = atoi(argv[2]);
	}

	// define threads
	pthread_t miners[count_of_miners];
	pthread_t traders[count_of_traders];

	// create mutex
	pthread_mutex_init(&mutex, NULL);

	// create threads for miners
	for(int i = 0; i < count_of_miners; i++) {
		int *p = malloc(sizeof(int));
		*p = i + 1;
		pthread_create(&miners[i], NULL, mine, p);
	}

	// create threads for traders
	for(int i = 0; i < count_of_traders; i++) {
		int *p = malloc(sizeof(int));
		*p = i + 1;
		pthread_create(&traders[i], NULL, trade, p);
	}

	// join miner threads
	for(int i = 0; i < count_of_miners; i++) {
		pthread_join(miners[i], NULL);
	}

	// join trader threads
	for(int i = 0; i < count_of_traders; i++) {
		pthread_join(traders[i], NULL);
	}

	// print remaining gold
	printf("Gold: %d", gold);

	// destroy mutex
	pthread_mutex_destroy(&mutex);
	
	return 0;
}
