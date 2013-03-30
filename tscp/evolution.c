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
#include "defs.h"
#include "data.h"
#include "protos.h"

int indiv_per_generation = 4;

void init_evolution() {
	FILE *evolution_file;
	evolution_file = open_file("evolution_individuals.txt");

	int i;
	for (i = 0 ; i < indiv_per_generation; i++) {
		evolution_individual test = retrieve_individual(evolution_file);
	}

	close_file(evolution_file);
}

/*
 * Provides the nest match to be player betweem two individuals
 */
void next_game() {


}


	/* Retrieve individuals in the evolution file */

	/* Set list of matchups */

	/* Play games */

	/* Compile results */

	/* Select the fittest for the next generation */

	/* Duplicate and mutate */

	/* Write in file */

	/* Write individuals in history file */
