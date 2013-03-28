/*
 * writer.c
 *
 *  Created on: Mar 27, 2013
 *      Author: cpguillo
 */

#include <stdio.h>
#include <stdlib.h>

FILE *evolution_file;

void open_file(char file_name[]) {
	evolution_file = fopen("evolution_individuals.txt", "r+");
	if (!evolution_file)
		printf("File not found.\n");
}

void close_file(FILE *file) {
	if (file)
		fclose(file);
	file = NULL;
}

void write_individual_values(FILE *file) {
	fprintf(file, "%d %d\n", 16, 35);

}

int main() {
	open_file("evolution_individuals.txt");

	char test[50];
	//read
	while (!feof(evolution_file)) {

		fscanf(evolution_file, "%s\n", test);
		printf("Content : %s\n", test);
	}

	//write
	/*printf("Entrez une chaine de carateres : ");
	scanf("%s", test);
	printf("%s\n", test);*/
	//fprintf(evolution_file, "%s\n", test);
	write_individual_values(evolution_file);

	close_file(evolution_file);
	return 0;
}

