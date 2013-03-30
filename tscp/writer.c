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

void open_file(FILE *file, char file_name[]) {
	printf("HIIIII");
	evo = fopen(file_name, "r+");
	if (!evo)
		printf("File not found.\n");

	file = fopen(file_name, "r+");
	if (!file)
		printf("File not found.\n");
}

void close_file(FILE *file) {
	if (file)
		fclose(file);
	file = NULL;
}

void write_individual_values(FILE *file, evolution_individual ei) {
	fprintf(file, "%d %d %d %d %d %d\n", ei.id, ei.pieces_value[0],
			ei.pieces_value[1], ei.pieces_value[2], ei.pieces_value[3],
			ei.pieces_value[4]);

}

evolution_individual retrieve_individual(FILE *file) {
	evolution_individual retrieved_individual;
	char line[50];
	char *split;
	printf("hello\n");
	if (!file) {
		printf("erreur");
	}
	char *test = fgets(line, 50, file);
	printf("Apres");
	printf("%s", line);
	split = strtok(line, " \n");
	int j;
	for (j = 0; j < 6; j++) {
		printf("%s\n", split);
		int parsedInt = strtol(split, NULL, 10);
		if (j == 0) {
			retrieved_individual.id = parsedInt;
		} else {
			retrieved_individual.pieces_value[j - 1] = parsedInt;
		}
		split = strtok(NULL, " \n");
	}
	printf("%d %d %d %d %d %d\n", retrieved_individual.id,
			retrieved_individual.pieces_value[0],
			retrieved_individual.pieces_value[1],
			retrieved_individual.pieces_value[2],
			retrieved_individual.pieces_value[3],
			retrieved_individual.pieces_value[4]);
	return retrieved_individual;
}

int main() {
	printf("Beginprogram");
	FILE *evolution_file = malloc(sizeof(struct FILE));
	open_file(evolution_file, "evolution_individuals.txt");
	char newline[50];
	fgets(newline, 50, evolution_file);

	char test[50];
	//read
	/*while (!feof(evolution_file)) {

	 fscanf(evolution_file, "%s\n", test);
	 printf("Content : %s\n", test);
	 }*/
	printf("Content before\n");
	evolution_individual eitest = retrieve_individual(evo);

	//write
	/*printf("Entrez une chaine de carateres : ");
	 scanf("%s", test);
	 printf("%s\n", test);*/
	//fprintf(evolution_file, "%s\n", test);
	evolution_individual individual = { .id = 1, .pieces_value = { 100, 300,
			300, 500, 900 } };

	//write_individual_values(evolution_file, individual);

	//close_file(evo);
	return 0;
}

