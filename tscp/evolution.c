/*
 * evolution.c
 *
 *  Created on: Mar 29, 2013
 *      Author: cpguillo
 *
 *      The structure of the file provides the next match that has to be
 *      played between two individuals (chess AI with different parameter values)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

#define MU			4
#define LAMBDA		8

//int indiv_per_generation = 4;
evolution_individual parents[MU];
evolution_individual mutants[LAMBDA];
evolution_individual sorting_array[LAMBDA + MU];

void init_evolution() {
	FILE *evolution_file;
	evolution_file = open_file("evolution_individuals.txt");
	int i;
	for(i = 0 ; i < MU; i++) {
		parents[i] = retrieve_individual(evolution_file);
	}
	close_file(evolution_file);
}

/* Creates the children from the parent generation */
void duplicate() {
	int i;
	for(i = 0 ; i < LAMBDA; i++) {
		memcpy(mutants[i], parents[(i % MU)], sizeof(evolution_individual));
	}
}

/* Mutates the values of the children */
void mutate() {
	srand(time(NULL));
	int i;
	for(i = 0 ; i < LAMBDA; i++) {
		int j;
		for(j = 0 ; j < 5; j++) {
			mutants[i]->pieces_value[j] += rand() % 10;
		}
	}
}

void determine_match_ups() {

}









/*
 * Provides the nest match to be player betweem two individuals
 */
void next_game() {


}

	/* Set list of matchups */

	/* Play games */

	/* Compile results */

	/* Select the fittest for the next generation */

	/* Write in file */

	/* Write individuals in history file */
