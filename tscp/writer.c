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

FILE *evo;

FILE* open_file(char file_name[]) {
	FILE *file = fopen(file_name, "r+");
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
	fprintf(file, "%d %d %d %d %d %d\n", ei->id, ei->pieces_value[0],
			ei->pieces_value[1], ei->pieces_value[2], ei->pieces_value[3],
			ei->pieces_value[4]);

}

evolution_individual* retrieve_individual(FILE *file) {
	evolution_individual *retrieved_individual = malloc(sizeof(evolution_individual));
	char line[50];
	char *split;
	fgets(line, 50, file);
	printf("%s", line);
	split = strtok(line, " \n");
	int j;
	for (j = 0; j < 6; j++) {
		printf("%s\n", split);
		int parsedInt = strtol(split, NULL, 10);
		if (j == 0) {
			retrieved_individual->id = parsedInt;
		} else {
			retrieved_individual->pieces_value[j - 1] = parsedInt;
		}
		split = strtok(NULL, " \n");
	}
	printf("%d %d %d %d %d %d\n", retrieved_individual->id,
			retrieved_individual->pieces_value[0],
			retrieved_individual->pieces_value[1],
			retrieved_individual->pieces_value[2],
			retrieved_individual->pieces_value[3],
			retrieved_individual->pieces_value[4]);
	return retrieved_individual;
}

int main() {
	FILE *evolution_file;
	evolution_file = open_file("evolution_individuals.txt");

	evolution_individual *eitest = retrieve_individual(evolution_file);
	eitest->pieces_value[1] = 5555555;

	//evolution_individual individual = { .id = 1, .pieces_value = { 100, 300,
	//		300, 500, 900 } };

	write_individual_values(evolution_file, eitest);

	close_file(evolution_file);
	return 0;
}

