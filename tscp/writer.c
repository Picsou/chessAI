/*
 * writer.c
 *
 *  Created on: Mar 27, 2013
 *      Author: cpguillo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

FILE* open_file(char file_name[], char mode[]) {
	FILE *file = fopen(file_name, mode);
	if (!file)
		printf("File not found.\n");
	return file;
}

void close_file(FILE *file) {
	if (file)
		fclose(file);
	file = NULL;
}

void write_individual_values(FILE *file, evolution_individual *ei) {
	fprintf(file, "%d %d %d %d %d %d %d\n", ei->id, ei->pieces_value[0],
			ei->pieces_value[1], ei->pieces_value[2], ei->pieces_value[3],
			ei->pieces_value[4], ei->fitness);

}

evolution_individual* retrieve_individual(FILE *file) {
	evolution_individual *retrieved_individual = malloc(sizeof(evolution_individual));
	retrieved_individual->fitness = 0;
	char line[50];
	char *split;
	fgets(line, 50, file);
	split = strtok(line, " \n");
	int j;
	for (j = 0; j < 6; j++) {
		int parsedInt = strtol(split, NULL, 10);
		if (j == 0) {
			retrieved_individual->id = parsedInt;
		} else {
			retrieved_individual->pieces_value[j - 1] = parsedInt;
		}
		split = strtok(NULL, " \n");
	}
	return retrieved_individual;
}
