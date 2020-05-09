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

// End game constants
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

//----------------------------------------------
// FUNCTION: init_command_center
// Sets values for command center and inits the mutex for the command center.
// PARAMETERS:
// command_center (type: struct command_center_t) -> the command center
//----------------------------------------------
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

//----------------------------------------------
// FUNCTION: destroy_command_center
// Destroys the mutex for the command center.
// PARAMETERS:
// command_center (type: struct command_center_t) -> the command center
//----------------------------------------------
void destroy_command_center(struct command_center_t* command_center) {
	// Destroy command_center_mutex
	if(pthread_mutex_destroy(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_destroy");
	}
}

//----------------------------------------------
// FUNCTION: init_mineral_block
// Sets values for a mineral block with given id and inits the mutex for the mineral block.
// PARAMETERS:
// mineral_block (type: struct mineral_block_t) -> the mineral block
// id (type: int) -> id of the mineral block
//----------------------------------------------
void init_mineral_block(struct mineral_block_t* mineral_block, int id) {
	
	// Set mineral block values
	mineral_block -> id = id;
	mineral_block -> minerals_left = MINERALS_IN_MINERAL_BLOCK;

	// Init mineral_block_mutex and check for possible error
	if(pthread_mutex_init(&mineral_block -> mineral_block_mutex, NULL) != 0) {
		perror("pthread_mutex_init center");
	}
}

//----------------------------------------------
// FUNCTION: destroy_mineral_block
// Destroys the mutex for a mineral block.
// PARAMETERS:
// mineral_block (type: struct mineral_block_t) -> the mineral_block
//----------------------------------------------
void destroy_mineral_block(struct mineral_block_t* mineral_block) {
	// Destroy mineral_block_mutex
	if(pthread_mutex_destroy(&mineral_block -> mineral_block_mutex) != 0) {
		perror("pthread_mutex_destroy");
	}
}


//----------------------------------------------
// FUNCTION: scv_work
// Work routine for every SCV.
// PARAMETERS:
// scv_info (type: struct scv_info_t) -> contains all info needed for the SCVs
//----------------------------------------------
void* scv_work(void* arg) {

	// Get scv_info 
	struct scv_info_t scv_info = *((struct scv_info_t *) arg); 

	// Free allocated memory
	free(arg);

	// Track to which mineral block has SCV gone
	int mineral_block_index = 0;

	// Number of mined minerals
	int mined_minerals;

	while(1) {

		do {

			// Check if SCV has gone through all mineral blocks and they have been busy
			if(mineral_block_index == scv_info.mineral_block_count) {
				mineral_block_index = 0;
			}

			// Lock command_center_mutex and check for possible error
			if(pthread_mutex_lock(&command_center.command_center_mutex) != 0) {
				perror("pthread_mutex_lock");
			}

			// Check if all blocks are empty
			if(command_center.empty_mineral_blocks == scv_info.mineral_block_count) {

				// Unlock command_center_mutex and check for possible error
				if(pthread_mutex_unlock(&command_center.command_center_mutex) != 0) {
					perror("pthread_mutex_unlock");
				}
				
				pthread_exit(NULL);
			}

			// Unlock command_center_mutex and check for possible error
			if(pthread_mutex_unlock(&command_center.command_center_mutex) != 0) {
				perror("pthread_mutex_unlock");
			}

			// Travel to mineral_block
			sleep(SCV_TRAVEL_TIME_TO_MINERAL_BLOCK);

			// Mined minerals by SCV
			mined_minerals = 0;

			// Try locking mineral_block_mutex and check for possible error
			if(pthread_mutex_trylock(&scv_info.mineral_blocks[mineral_block_index].mineral_block_mutex) == 0) {

				// Check if current block has minerals
				if(scv_info.mineral_blocks[mineral_block_index].minerals_left != 0) {

					// Task requirements
					printf("SCV %d is mining from mineral block %d\n", scv_info.id, mineral_block_index + 1);

					// If there are less than 8 minerals to mine
					if(scv_info.mineral_blocks[mineral_block_index].minerals_left <= MINERALS_PER_MINE) {

						// Mine minerals
						mined_minerals = scv_info.mineral_blocks[mineral_block_index].minerals_left;
						scv_info.mineral_blocks[mineral_block_index].minerals_left = 0;
						
						// Lock command_center_mutex and check for possible error
						if(pthread_mutex_lock(&command_center.command_center_mutex) != 0) {
							perror("pthread_mutex_lock");
						}

						// Mark mineral block as empty
						command_center.empty_mineral_blocks += 1;

						// Unlock command_center_mutex and check for possible error
						if(pthread_mutex_unlock(&command_center.command_center_mutex) != 0) {
							perror("pthread_mutex_unlock");
						}

					}
					// If there are more than 8 minerals to mine
					else {
						// Mine minerals
						mined_minerals = MINERALS_PER_MINE;
						scv_info.mineral_blocks[mineral_block_index].minerals_left -= mined_minerals;
					}

				}

				// Unlock mineral_block_mutex and check for possible error
				if(pthread_mutex_unlock(&scv_info.mineral_blocks[mineral_block_index].mineral_block_mutex) != 0) {
					perror("pthread_mutex_unlock");
				}

			}

			// Increment mineral_block_index if current mineral block is busy
			mineral_block_index ++;

		} while(mined_minerals == 0);


		// Task requirements
		printf("SCV %d is transporting minerals\n", scv_info.id);

		// Transport minerals to command center
		sleep(SCV_TRANSPORTATION_TIME_TO_COMMAND_CENTER);

		// Lock command_center_mutex and check for possible error
		if(pthread_mutex_lock(&command_center.command_center_mutex) != 0) {
			perror("pthread_mutex_lock");
		}

		// Task requirements
		command_center.minerals += mined_minerals;
		printf("SCV %d delivered minerals to the Command Center\n", scv_info.id);

		// Unlock command_center_mutex and check for possible error
		if(pthread_mutex_unlock(&command_center.command_center_mutex) != 0) {
			perror("pthread_mutex_unlock");
		}

	}

	return NULL;

}

//----------------------------------------------
// FUNCTION: buy_marine
// Checks if player has enough minerals and buys a new marine
// PARAMETERS:
// command_center (type: struct command_center_t) -> the command center
//----------------------------------------------
void buy_marine(struct command_center_t* command_center) {

	// Lock mutex and check for possible error
	if(pthread_mutex_lock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_lock");
	}

	// Check if there are enough minerals
	if(command_center -> minerals < 50){
		
		// Task requirements
		printf("Not enough minerals.\n");

		// Unlock mutex and check for possible error
		if(pthread_mutex_unlock(&command_center -> command_center_mutex) != 0) {
			perror("pthread_mutex_unlock");
		}

		return;
	}

	// Unlock mutex and check for possible error
	if(pthread_mutex_unlock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_unlock");
	}

	// Task requirements
	sleep(MARINE_TRAINING_TIME);
	
	// Lock mutex and check for possible error
	if(pthread_mutex_lock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_lock");
	}

	// Task requirements
	command_center -> minerals -= 50;
	command_center -> marine_count ++;
	
	// Unlock mutex and check for possible error
	if(pthread_mutex_unlock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_unlock");
	}

	// Task requirements
	printf("You wanna piece of me, boy?\n");
}

//----------------------------------------------
// FUNCTION: buy_scv
// Checks if player has enough minerals and buys a new SCV
// PARAMETERS:
// command_center (type: struct command_center_t) -> the command center
// player_info (type: struct scv_info_t) -> contains all info that will be passed to the new SCV
//----------------------------------------------
void buy_scv(struct command_center_t* command_center, struct scv_info_t* player_info) {
	
	// Lock mutex and check for possible error
	if(pthread_mutex_lock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_lock");
	}

	// Check if there are enough minerals
	if(command_center -> minerals < 50){
		
		// Task requirements
		printf("Not enough minerals.\n");

		// Unlock mutex and check for possible error
		if(pthread_mutex_unlock(&command_center -> command_center_mutex) != 0) {
			perror("pthread_mutex_unlock");
		}

		return;
	}

	// Unlock mutex and check for possible error
	if(pthread_mutex_unlock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_unlock");
	}

	// Task requirements
	sleep(SCV_TRAINING_TIME);

	// Create new variable so player info is saved for more SCVs
	struct scv_info_t* scv_info = malloc(sizeof(struct scv_info_t));

	scv_info -> mineral_blocks = player_info -> mineral_blocks;
	scv_info -> mineral_block_count = player_info -> mineral_block_count;

	// Lock mutex and check for possible error
	if(pthread_mutex_lock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_lock");
	}

	// Task requirements
	command_center -> minerals -= 50;
	command_center -> scv_count ++;

	// Allocate memory for new SCV
	scv = realloc(scv, command_center -> scv_count * sizeof(pthread_t));

	scv_info -> id = command_center -> scv_count;

	// Create thread for the new SCV
	if(pthread_create(&scv[command_center -> scv_count - 1], NULL, scv_work, scv_info) != 0) {
		perror("pthread_create");
		exit(-1);
	}

	// Unlock mutex and check for possible error
	if(pthread_mutex_unlock(&command_center -> command_center_mutex) != 0) {
		perror("pthread_mutex_unlock");
	}

	// Task requirements
	printf("SCV good to go, sir.\n");

}


//----------------------------------------------
// FUNCTION: play
// Routine of the player.
// PARAMETERS:
// player_info (type: struct scv_info_t) -> stores a copy of scv_info to pass to every new SCV
//----------------------------------------------
void* play(void* arg) {

	// Get scv_info 
	struct scv_info_t player_info = *((struct scv_info_t *) arg); 

	// Free allocated memory
	free(arg);

	while(1) {

		// Lock mutex and check for possible error
		if(pthread_mutex_lock(&command_center.command_center_mutex) != 0) {
			perror("pthread_mutex_lock");
		}

		// Check if marine limit has been reached
		if(command_center.marine_count == MARINE_LIMIT) {

			// Unlock mutex and check for possible error
			if(pthread_mutex_unlock(&command_center.command_center_mutex) != 0) {
				perror("pthread_mutex_unlock");
			}
			
			pthread_exit(NULL);

		}

		// Unlock mutex and check for possible error
		if(pthread_mutex_unlock(&command_center.command_center_mutex) != 0) {
			perror("pthread_mutex_unlock");
		}

		// Get user input
		char c = getchar();

		// Check which command user has entered
		switch(c) {
			case BUY_MARINE_COMMAND:
				buy_marine(&command_center);
				break;
			case BUY_SCV_COMMAND:
				buy_scv(&command_center, &player_info);
				break;
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
	scv = malloc(command_center.scv_count * sizeof(pthread_t));

	// Create all SCV threads and pass SCV info
	for(int i = 0; i < command_center.scv_count; i++) {
		
		// Set SCV arguments
		struct scv_info_t* scv_info = malloc(sizeof(struct scv_info_t));
		scv_info -> id = i + 1;
		scv_info -> mineral_blocks = mineral_blocks;
		scv_info -> mineral_block_count = mineral_block_count;

		// Create SCV thread
		if(pthread_create(&scv[i], NULL, scv_work, scv_info) != 0) {
			perror("pthread_create");
			exit(-1);
		}
	}

	// Allocate memory for player info
	struct scv_info_t* player_info = malloc(sizeof(struct scv_info_t));

	// Set player arguments	
	player_info -> mineral_blocks = mineral_blocks;
	player_info -> mineral_block_count = mineral_block_count;

	// Create player thread
	if(pthread_create(&player, NULL, play, player_info) != 0) {
		perror("pthread_create");
	}

	// Join all SCV threads
	for(int i = 0; i < command_center.scv_count; i++) {
		if(pthread_join(scv[i], NULL) != 0) {
			perror("pthread_join");
			exit(-1);
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
