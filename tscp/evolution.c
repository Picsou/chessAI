/*
 * evolution.c
 *
 *  Created on: Mar 29, 2013
 *      Author: cpguillo
 *
 *      The structure of the file provides the next match that has to be
 *      played between two individuals (chess AI with different parameter values).
 *      The games between different AI is the selection process in itself.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

#define MU			4
#define LAMBDA		8

int individual_id = 4; // the original individuals are retrieved from the evolution file
int first_time = 1; // Special case initialization for the evolution strategy
evolution_individual* parents[MU];
evolution_individual* mutants[LAMBDA];
evolution_individual* sorting_array[LAMBDA + MU];
match_up match_ups_list[(LAMBDA + MU - 1) * (LAMBDA + MU)]; // Each AI plays white and black against another AI
int played_match_ups;

void init_evolution() {
	printf("Begin init\n");
	FILE *evolution_file;
	evolution_file = open_file("evolution_individuals.txt", "r");
	int i;
	for (i = 0; i < MU; i++) {
		parents[i] = retrieve_individual(evolution_file);
	}
	close_file(evolution_file);
	printf("End init\n");
}

/* Creates the children from the parent generation */
void duplicate() {
	printf("Begin duplicate\n");
	int i;
	for (i = 0; i < LAMBDA; i++) {
		evolution_individual *mutant = malloc(sizeof(evolution_individual));
		printf("inside");
		memcpy(mutant, parents[(i % MU)], sizeof(evolution_individual));
		mutants[i] = mutant;
		mutants[i]->id = individual_id;
		individual_id++;
	}
	printf("End duplicate\n");
}

/* Mutates the values of the children */
void mutate() {
	printf("Begin mutate\n");
	srand(time(NULL));
	int i;
	for (i = 0; i < LAMBDA; i++) {
		int j;
		printf("1st loop\n");
		for (j = 0; j < 5; j++) {
			printf("2nd loop\n");
			mutants[i]->pieces_value[j] += rand() % 10;
		}
	}
	printf("End mutate\n");
}

/* Parents and children will play altogether to determine the fittest individuals */
void determine_match_ups() {
	printf("Begin matchup\n");
	// Fill up the array that will serve for sorting the different AIs after their matches
	int i;
	for (i = 0; i < MU; i++) {
		evolution_individual *mutant = malloc(sizeof(evolution_individual));
		memcpy(mutant, parents[i], sizeof(evolution_individual));
		sorting_array[i] = mutant;
	}
	int j;
	for (j = 0; j < LAMBDA; j++) {
		evolution_individual *mutant = malloc(sizeof(evolution_individual));
		memcpy(mutant, mutants[j], sizeof(evolution_individual));
		sorting_array[MU + j] = mutant;
	}

	// Store match-ups between the AIs
	for (i = 0; i < MU + LAMBDA; i++) {
		for (j = 0; j < MU + LAMBDA - 1; j++) {
			match_ups_list[i + j].white = sorting_array[i];
			match_ups_list[i + j].black = sorting_array[(i + j + 1) % (MU
					+ LAMBDA)];
		}
	}
	printf("End matchup\n");
}

void compile_results() {
	printf("Begin compile\n");
	/* Initialisation of the number of victories */
	int i;
	for (i = 0; i < LAMBDA + MU; i++) {
		sorting_array[i]->fitness = 0;
	}

	/* Counting the number of victories */
	for (i = 0; i < (LAMBDA + MU - 1) * (LAMBDA + MU); i++) {
		int j;
		for (j = 0; j < LAMBDA + MU; j++) {
			if (sorting_array[j]->id == match_ups_list[i].winner) {
				printf("%d   :    %d\n", sorting_array[j]->id, match_ups_list[i].winner);
				if (sorting_array[j] == NULL) {
					printf("merde\n");
				}
				sorting_array[j]->fitness++;
			}
		}
	}
	printf("End compile\n");
}

/* The parent array becomes the array of parents for the next generation. Selection occurs through
 * a bubble sort based on the fitness of each AI. */
void selection() {
	printf("Begin selection\n");
	int n = MU + LAMBDA;
	int swapped;
	do {
		swapped = 0;
		int i;
		for (i = 1; i < n; i++) {
			printf("%d\n", sorting_array[i - 1]->fitness);
			if (sorting_array[i - 1]->fitness > sorting_array[i]->fitness) {
				evolution_individual *temp = sorting_array[i - 1];
				sorting_array[i - 1] = sorting_array[i];
				sorting_array[i] = temp;
				swapped = 1;
				printf("midmid\n");
			}
		}
		n--;
	} while (!swapped);

	printf("Middle Selection\n");
	/* Write the surviving individuals into the evolution file and the history file */
	/* Evolution file */
	FILE *evolution_file = open_file("evolution_individuals.txt", "w");
	int i;
	for (i = 0; i < MU; i++) {
		write_individual_values(evolution_file, sorting_array[LAMBDA + i]);
	}
	close_file(evolution_file);

	/* History file */
	FILE *history_file = open_file("history_individuals.txt", "a");
	for (i = 0; i < MU; i++) {
		write_individual_values(history_file, sorting_array[LAMBDA + i]);
	}
	close_file(history_file);
	printf("End selection\n");
}

/*
 * Provides the nest match to be player betweem two individuals
 */
match_up next_game() {
	if (first_time) {
		printf("first time\n");
		first_time = 0;
		init_evolution();
		duplicate();
		mutate();
		determine_match_ups();
		played_match_ups = 0;
		return match_ups_list[played_match_ups];
	} else if (played_match_ups >= LAMBDA + MU - 1) { // A new generation is going to be bred
		// Processing the results from the previous generation
		printf("games finished\n");
		compile_results();
		selection();

		// Breeding a new generation
		init_evolution();
		duplicate();
		mutate();
		determine_match_ups();
		played_match_ups = 0;
		return match_ups_list[played_match_ups];
	} else {
		printf("Next game\n");
		played_match_ups++;
		return match_ups_list[played_match_ups];
	}

}
