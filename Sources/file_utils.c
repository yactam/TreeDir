#include "../Headers/file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

FILE* file_reader(char* nomFic) {
	FILE* flux = fopen(nomFic, "r");
	if(flux == NULL) {
		perror("Probleme dans l'ouverture du fichier");
		exit(EXIT_FAILURE);
	}
	return flux;
}

size_t nbCommands(FILE* flux) {
	size_t ret = 0;
	assert(flux != NULL);
	int r = fseek(flux, 0, SEEK_SET);
	assert(r == 0);
	int c = 0;
	while((c = fgetc(flux)) != EOF) {
		if(c == '\n') ++ret;
	}
	return ret;
}

char** getCommands(FILE* flux) {
	size_t size = nbCommands(flux);
	char** res = malloc(size * sizeof(char*));
	assert(flux != NULL);
	assert(fseek(flux, 0, SEEK_SET) == 0);
	int r = 0, cour = 0, dec = 0, i = 0, index = 0;
	char* st = "";
	while((i = fgetc(flux)) != EOF) {
		if(i != '\n') {
			++dec;
		} else {
			r = fseek(flux, cour, SEEK_SET);
			assert(r == 0);
			st = malloc(sizeof(char) * (dec+2));
			char* st2 = fgets(st, dec+2, flux);
			assert(st2 != NULL);
			size_t l = strlen(st);
			res[index] = malloc(l * sizeof(char));
			assert(memcpy(res[index], st, (l-1) * sizeof(char)) != NULL);
			*(res[index]+(l-1)) = '\0';
			cour += dec+1;
			dec = 0;
			index++;
		}
	}
	return res;
}

void close_file(FILE* fic) {
	if(fclose(fic) != 0) {
		perror("Probleme dans la fermeture de fichier");
		exit(EXIT_FAILURE);
	}
}