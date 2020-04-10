//--------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE NAME: starcraft3.c
// FILE PURPOSE:
// Basic implementation of Starcraft 3
//---------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Default values at the start of the game
#define DEFAULT_SCV_COUNT 5
#define DEFAULT_MARINE_COUNT 0
#define DEFAULT_MINERAL_BLOCK_COUNT 2

// Time related constants
#define SCV_TRAINING_TIME 4
#define MARINE_TRAINING_TIME 1
#define SCV_TRAVEL_TIME_TO_MINERAL_BLOCK 3
#define SCV_TRANSPORTATION_TIME_TO_COMMAND_CENTER 2

// Price list
#define SCV_COST 50
#define MARINE_COST 50

// Mineral related constants
#define MINERALS_PER_MINE 8
#define MINERALS_IN_MINERAL_BLOCK 500

// End game values
#define MARINE_LIMIT 20

// User commands
#define BUY_SCV_COMMAND 's'
#define BUY_MARINE_COMMAND 'm'

// Struct for the command center
struct command_center_t {
	int minerals;
	int scv_count;
	int marine_count;
	int empty_mineral_blocks;
	pthread_mutex_t command_center_mutex;
};

// Struct for every mineral block
struct mineral_block_t {
	int id;
	int minerals_left;
	pthread_mutex_t mineral_block_mutex;
};

// Struct containing SCV info
struct scv_info_t {
	int id;
	int mineral_block_count;
	struct mineral_block_t* mineral_blocks;
};

// Command center
struct command_center_t command_center;

// SCV threads
pthread_t* scv;

// Player thread
pthread_t player;

void buy_marine(struct command_center_t* command_center) {
	// Check if there are enough minerals
	if(command_center -> minerals < 50){
		printf("Not enough minerals.\n");
		return;
	}
	
	// Lock mutex and check for possible error
	if(pthread_mutex_lock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_lock");
	}

	command_center -> minerals -= 50;
	command_center -> marine_count ++;
	
	// Unlock mutex and check for possible error
	if(pthread_mutex_unlock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_unlock");
	}

	// Task requirements
	sleep(MARINE_TRAINING_TIME);
	printf("You wanna piece of me, boy?\n");
}

void init_command_center(struct command_center_t* command_center) {
	
	// Set command center values
	command_center -> minerals = 0;
	command_center -> scv_count = DEFAULT_SCV_COUNT;
	command_center -> marine_count = DEFAULT_MARINE_COUNT;
	command_center -> empty_mineral_blocks = 0;

	// Init command_center_mutex and check for possible error
	if(pthread_mutex_init(&command_center -> command_center_mutex, NULL) != 0) {
		perror("pthread_mutex_init");
	}
}

void destroy_command_center(struct command_center_t* command_center) {
	// Destroy command_center_mutex
	if(pthread_mutex_destroy(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_destroy");
	}
}

void init_mineral_block(struct mineral_block_t* mineral_block, int id) {
	
	// Set mineral block values
	mineral_block -> id = id;
	mineral_block -> minerals_left = MINERALS_IN_MINERAL_BLOCK;

	// Init mineral_block_mutex and check for possible error
	if(pthread_mutex_init(&mineral_block -> mineral_block_mutex, NULL) != 0) {
		perror("pthread_mutex_init");
	}
}

void destroy_mineral_block(struct mineral_block_t* mineral_block) {
	// Destroy mineral_block_mutex
	if(pthread_mutex_destroy(&mineral_block -> mineral_block_mutex) != 0) {
		perror("pthread_mutex_destroy");
	}
}

void* scv_work(void* arg) {

	// Get scv_info 
	struct scv_info_t scv_info = *((struct scv_info_t *) arg); 

	// Free allocated memory
	free(arg);

	// Track to which mineral block has SCV gone
	int mineral_block_index = 0;

	// Number of mined minerals
	int mined_minerals;

	while(21) {

		do {

			// Travel to mineral_block
			sleep(SCV_TRAVEL_TIME_TO_MINERAL_BLOCK);

			mined_minerals = 0;

			// Check if SCV has gone through all mineral blocks and they have been busy
			if(mineral_block_index == scv_info.mineral_block_count) {
				mineral_block_index = 0;
			}

			// Check if all blocks are empty
			if(command_center.empty_mineral_blocks == scv_info.mineral_block_count) {
				pthread_exit(NULL);
			}

			// Try locking mineral_block_mutex and check for possible error
			if(pthread_mutex_trylock(&scv_info.mineral_blocks[mineral_block_index].mineral_block_mutex) == 0) {

				// Check if current block is empty
				if(scv_info.mineral_blocks[mineral_block_index].minerals_left == 0) {
					continue;
				}

				// Task requirements
				printf("SCV %d is mining from mineral block %d\n", scv_info.id, mineral_block_index + 1);

				// If there are less than 8 minerals to mine
				if(scv_info.mineral_blocks[mineral_block_index].minerals_left < MINERALS_PER_MINE) {

					// Mine minerals
					mined_minerals = scv_info.mineral_blocks[mineral_block_index].minerals_left;
					scv_info.mineral_blocks[mineral_block_index].minerals_left = 0;
					
					// Mark mineral block as empty
					command_center.empty_mineral_blocks += 1;
				}
				// If there are more than 8 minerals to mine
				else {
					// Mine minerals
					mined_minerals = MINERALS_PER_MINE;
					scv_info.mineral_blocks[mineral_block_index].minerals_left -= mined_minerals;
				}

				// Unlock mineral_block_mutex and check for possible error
				if(pthread_mutex_unlock(&scv_info.mineral_blocks[mineral_block_index].mineral_block_mutex) != 0) {
					perror("pthread_mutex_unlock");
				}

			}

			mineral_block_index ++;

		} while(mined_minerals == 0);

		// Task requirements
		printf("SCV %d is transporting minerals\n", scv_info.id);

		// Transport minerals to command center
		sleep(SCV_TRANSPORTATION_TIME_TO_COMMAND_CENTER);

		if(pthread_mutex_lock(&command_center.command_center_mutex) != 0) {
			perror("pthread_mutex_lock");
		}

		// Task requirements
		printf("SCV %d delivered minerals to Command center\n", scv_info.id);
		command_center.minerals += mined_minerals;

		if(pthread_mutex_unlock(&command_center.command_center_mutex) != 0) {
			perror("pthread_mutex_unlock");
		}

	}

	return NULL;

}

void* play() {

	while(command_center.marine_count < MARINE_LIMIT) {

		char c = getchar();

		switch(c) {
			case BUY_MARINE_COMMAND:
				buy_marine(&command_center);
				break;
			// case BUY_SCV_COMMAND:
				// buy_scv(&command_center);
				// break;
			default:
				break;
		}

	}

	return NULL;

}

int main(int argc, char const *argv[]) {

	// Initialize command center
	init_command_center(&command_center);

	// Get count of mineral blocks
	int mineral_block_count = (argc == 1 ? DEFAULT_MINERAL_BLOCK_COUNT : atoi(argv[1]));

	// Array with mineral blocks
	struct mineral_block_t mineral_blocks[mineral_block_count];

	// Initialize mineral blocks
	for(int i = 0; i < mineral_block_count; i++) {
		init_mineral_block(&mineral_blocks[i], i + 1);
	}

	// Allocate memory for SCVs
	scv = (pthread_t *) malloc(DEFAULT_SCV_COUNT * sizeof(pthread_t));

	// Create all SCV threads and pass SCV info
	for(int i = 0; i < DEFAULT_SCV_COUNT; i++) {
		struct scv_info_t *scv_info = malloc(sizeof(struct scv_info_t));
		scv_info -> id = i + 1;
		scv_info -> mineral_blocks = mineral_blocks;
		scv_info -> mineral_block_count = mineral_block_count;

		if(pthread_create(&scv[i], NULL, scv_work, scv_info) != 0) {
			perror("pthread_create");
		}
	}

	// Create player thread
	if(pthread_create(&player, NULL, play, NULL) != 0) {
		perror("pthread_create");
	}

	// Join all SCV threads
	for(int i = 0; i < DEFAULT_SCV_COUNT; i++) {
		if(pthread_join(scv[i], NULL) != 0) {
			perror("pthread_join");
		}
	}

	// Join player thread
	if(pthread_join(player, NULL) != 0) {
		perror("pthread_join");
	}

	// Task requirements
	printf("Map minerals %d, ", mineral_block_count * MINERALS_IN_MINERAL_BLOCK);
	printf("player minerals %d, ", command_center.minerals);
	printf("SCVs %d, ", command_center.scv_count);
	printf("Marines %d\n", command_center.marine_count);

	// Free allocated memory for SCVs
	free(scv);

	// Destroy command center
	destroy_command_center(&command_center);

	// Destroy all mineral blocks
	for(int i = 0; i < mineral_block_count; i++) {
		destroy_mineral_block(&mineral_blocks[i]);
	}

	return 0;
}