//--------------------------------------------
// NAME: Ognian Baruh
// CLASS: XI A
// NUMBER: 21
// PROBLEM: #1
// FILE PURPOSE: 
// Zombies game
//---------------------------------------------

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "ui.h"

#define MINER_PRICE 100
#define SOLDIER_PRICE 10
#define GOLD_GATHERED_BY_MINER 10
#define SLEEP_TIME 1
#define NEW_MINERS 10

// declare mutex;
pthread_mutex_t mutex;

// array with miners' thread
pthread_t miners[200];

// keeps miner_index of next thread which is free
int miner_index = 0;

// thread for zombies
pthread_t zombies;

// self explanatory (I guess)
int player_health = 100;
unsigned int gold = 100;
unsigned int zombies_count = 1;
unsigned int soldiers_count = 0;

void* mine() {
	while(21) {
		// lock mutex
		pthread_mutex_lock(&mutex);

		// mine gold
		gold += GOLD_GATHERED_BY_MINER;

		// unlock mutex
		pthread_mutex_unlock(&mutex);

		// print gold
		print_gold(gold);

		// sleep
		sleep(SLEEP_TIME);
	}

	return NULL;
}

void* attack() {
	while(21) {
		for(int i = 5; i >= 0; i--) {
			// print zombies count
			print_zombies(i, zombies_count);

			// sleep
			sleep(SLEEP_TIME);
		}

		// lock mutex since soldiers_count shouldn't change during the check
		pthread_mutex_lock(&mutex);

		if(zombies_count > soldiers_count) {
			// take out from player's health
			player_health -= (zombies_count - soldiers_count);

			// message for failed defence 
			print_fail("Zombie attack succeded ;(!");

			// check if player is dead
			if(player_health <= 0) {
				// end the game
				game_end(zombies_count);
			}

			// print remaining health
			print_health(player_health);
		}
		else {
			// message for successful defence
			print_succ("Zombie attack deflected! :)");
		}

		// unlock mutex
		pthread_mutex_unlock(&mutex);

		// new zombies
		zombies_count *= 2;

		// print new zombies_count
		print_zombies(5, zombies_count);
	}

	return NULL;
}

void add_miner() {
	// check if player has enough gold
	if(gold < MINER_PRICE) {
		// print error message
		print_fail("Not enough gold!");
		return;
	}

	// create thread for this miner
	pthread_create(&miners[miner_index], NULL, mine, NULL);

	// increment miner_index
	miner_index += 1;

	// lock mutex
	pthread_mutex_lock(&mutex);

	// tax the player
	gold -= MINER_PRICE;

	// unlock mutex
	pthread_mutex_unlock(&mutex);

	// print gold
	print_gold(gold);

	// print success message
	print_msg("Miner created!");
}

void add_soldiers(int count) {
	// check if player has enough gold
	if(gold < SOLDIER_PRICE * count) {
		// print error message
		print_fail("Not enough gold!");
		return;
	}

	// lock mutex
	pthread_mutex_lock(&mutex);

	// add soldiers to soldiers_count
	soldiers_count += count;

	// tax the player
	gold -= SOLDIER_PRICE * count;

	// unlock mutex
	pthread_mutex_unlock(&mutex);

	// print current soldiers_count
	print_soldiers(soldiers_count);

	// print current gold
	print_gold(gold);

	// check which print message to print
	if(count == 1) {
		// print success message
		print_msg("Soldier created!");
	}
	else {
		// print success message
		print_msg("10 x soldiers created!");
	}
}

int main() {
	// initialize game
	init();
	print_gold(gold);
	print_soldiers(soldiers_count);
	print_zombies(5, zombies_count);
	print_health(player_health);

	// create zombies' thread
	pthread_create(&zombies, NULL, attack, NULL);

	while(21) {
		// get user's input
		int ch = get_input();
		switch(ch) {
			case 'm':
				add_miner();
				break;
			case 's':
				add_soldiers(1);
				break;
			case 'x':
				add_soldiers(10);
				break;
			case 'q':
				game_end(1);
				break;
			default:
				break;
		}
	}
}
