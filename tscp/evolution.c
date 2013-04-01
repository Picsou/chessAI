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

int first_time = 1; // Special case initialization for the evolution strategy
evolution_individual* parents[MU];
evolution_individual* mutants[LAMBDA];
evolution_individual* sorting_array[LAMBDA + MU];
match_up match_ups_list[(LAMBDA + MU - 1) * (LAMBDA + MU)]; // Each AI plays white and black against another AI
int played_match_ups;

void init_evolution() {
	FILE *evolution_file;
	evolution_file = open_file("evolution_individuals.txt", "r");
	int i;
	for (i = 0; i < MU; i++) {
		parents[i] = retrieve_individual(evolution_file);
	}
	close_file(evolution_file);
}

/* Creates the children from the parent generation */
void duplicate() {
	int i;
	for (i = 0; i < LAMBDA; i++) {
		evolution_individual *mutant;
		memcpy(mutant, parents[(i % MU)], sizeof(evolution_individual));
	}
}

/* Mutates the values of the children */
void mutate() {
	srand(time(NULL));
	int i;
	for (i = 0; i < LAMBDA; i++) {
		int j;
		for (j = 0; j < 5; j++) {
			mutants[i]->pieces_value[j] += rand() % 10;
		}
	}
}

/* Parents and children will play altogether to determine the fittest individuals */
void determine_match_ups() {
	// Fill up the array that will serve for sorting the different AIs after their matches
	int i;
	for (i = 0; i < MU; i++) {
		memcpy(sorting_array[i], parents[i], sizeof(evolution_individual));
	}
	int j;
	for (j = 0; j < LAMBDA; j++) {
		memcpy(sorting_array[MU + j], mutants[j], sizeof(evolution_individual));
	}

	// Store match-ups between the AIs
	for (i = 0; i < MU + LAMBDA; i++) {
		for (j = 0; j < MU + LAMBDA - 1; j++) {
			match_ups_list[i + j].white = sorting_array[i];
			match_ups_list[i + j].black = sorting_array[(i + j + 1) % (MU
					+ LAMBDA)];
		}
	}

}

void compile_results() {
	int i;
	for (i = 0; i < (LAMBDA + MU - 1) * (LAMBDA + MU); i++) {
		int j;
		for (j = 0; j < LAMBDA + MU; j++) {
			if (sorting_array[j]->id == match_ups_list[i].winner) {
				sorting_array[j]->fitness++;
			}
		}
	}
}

/* The parent array becomes the array of parents for the next generation. Selection occurs through
 * a bubble sort based on the fitness of each AI. */
void selection() {
	int n = MU + LAMBDA;
	int swapped;
	do {
		swapped = 0;
		int i;
		for (i = 1; i < n; i++) {
			if (sorting_array[i - 1]->fitness > sorting_array[i]->fitness) {
				evolution_individual *temp = sorting_array[i - 1];
				sorting_array[i - 1] = sorting_array[i];
				sorting_array[i] = temp;
				swapped = 1;
			}
		}
		n--;
	} while (!swapped);

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
}

/*
 * Provides the nest match to be player betweem two individuals
 */
match_up next_game() {
	if (first_time) {
		first_time = 0;
		init_evolution();
		duplicate();
		mutate();
		determine_match_ups();
		played_match_ups = 0;
		return match_ups_list[played_match_ups];
	} else if (played_match_ups >= LAMBDA + MU - 1) { // A new generation is going to be bred
		// Processing the results from the previous generation
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
		played_match_ups++;
		return match_ups_list[played_match_ups];
	}

}
