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

/* Evolution parameters */
#define MU			4
#define LAMBDA		8

int individual_id = 4; /* The original individuals (0,1,2,3) are retrieved from the evolution file */
int begin_time = 0;
int evolution_time = 86400000; /* Running time of the evolution algorithm (the last iteration finishes before leaving the program) */
int number_generation = 0;

/* AIs' breeding */
int first_time = 1; /* Special case initialization for the evolution strategy */
evolution_individual* parents[MU];
evolution_individual* mutants[LAMBDA];
evolution_individual* sorting_array[LAMBDA + MU];

/* Competition list */
match_up* match_ups_list[(LAMBDA + MU - 1) * (LAMBDA + MU)]; /* Each AI plays white and black against another AI */
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
		evolution_individual *mutant = malloc(sizeof(evolution_individual));
		memcpy(mutant, parents[(i % MU)], sizeof(evolution_individual));
		mutants[i] = mutant;
		mutants[i]->id = individual_id;
		individual_id++;
	}
}

/* Mutates the values of the children */
void mutate() {
	srand(time(NULL));
	int i;
	for (i = 0; i < LAMBDA; i++) {
		int j;
		for (j = 0; j < 5; j++) {
			mutants[i]->pieces_value[j] += (rand() % 30) - 15;
		}
	}
}

/* Parents and children will play altogether to determine the fittest individuals */
void determine_match_ups() {
	/* Fill up the array that will serve for sorting the different AIs after their matches */
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

	/* Store match-ups between the AIs */
	int match_up_position = 0;
	for (i = 0; i < MU + LAMBDA; i++) {
		for (j = 0; j < MU + LAMBDA - 1; j++) {
			match_up *game = malloc(sizeof(match_up));
			game->white = sorting_array[i];
			game->black = sorting_array[(i + j + 1) % (MU + LAMBDA)];
			match_ups_list[match_up_position] = game;
			match_up_position++;
		}
	}
}

void compile_results() {
	/* Initialisation of the number of victories */
	int i;
	for (i = 0; i < LAMBDA + MU; i++) {
		sorting_array[i]->fitness = 0;
	}

	/* Counting the number of victories */
	for (i = 0; i < (LAMBDA + MU - 1) * (LAMBDA + MU); i++) {
		if (match_ups_list[i]->winner == match_ups_list[i]->white->id) {
			match_ups_list[i]->white->fitness += 3;
		} else if (match_ups_list[i]->winner == match_ups_list[i]->black->id) {
			match_ups_list[i]->black->fitness += 3;
		} else { /* stalemate or draw */
			match_ups_list[i]->white->fitness += 1;
			match_ups_list[i]->black->fitness += 1;
		}
	}
}

/* Ordering function for sorting the individuals through their number of victories */
int compare(void const *a, void const *b) {
	evolution_individual *pa = *(evolution_individual * const *) a;
	evolution_individual *pb = *(evolution_individual * const *) b;
	return pa->fitness - pb->fitness;
}

/* Selects the MU most fitted individuals */
void selection() {
	qsort(sorting_array, LAMBDA + MU, sizeof(sorting_array[0]), compare);

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
 * Provides the nest match to be played between two individuals
 */
match_up* next_game() {
	if (first_time) {
		begin_time = get_ms();
		first_time = 0;
		init_evolution();
		duplicate();
		mutate();
		determine_match_ups();
		played_match_ups = 0;
		return match_ups_list[played_match_ups];
	} else if (played_match_ups >= (LAMBDA + MU - 1) * (LAMBDA + MU) - 1) { /* A new generation is going to be bred */
		/* Processing the results from the previous generation */
		compile_results();
		selection();
		number_generation++;
		if (get_ms() - begin_time > evolution_time) { /* The simulation ran for the required time by the user */
			/* Freeing the structures */
			int i;
			for (i = 0; i < MU; i++) {
				free(parents[i]);
			}
			for (i = 0; i < LAMBDA; i++) {
				free(mutants[i]);
			}
			for (i = 0; i < MU + LAMBDA; i++) {
				free(sorting_array[i]);
			}
			for (i = 0; i < (LAMBDA + MU - 1) * (LAMBDA + MU); i++) {
				free(match_ups_list[i]);
			}
			printf("Number of generations : %d\n", number_generation);
			exit(0);
		}

		/* Breeding a new generation */
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
