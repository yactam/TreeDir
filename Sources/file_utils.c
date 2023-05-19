#include "../Headers/file_utils.h"
#include "../Headers/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

FILE* file_reader(char* nomFic) {
	FILE* flux = fopen(nomFic, "r");
	check(flux, "Error in file opening.");
	return flux;
error:
	exit(EXIT_FAILURE);
}

size_t nbCommands(FILE* flux) {
	size_t ret = 0;
	assert(flux != NULL);
	int r = fseek(flux, 0, SEEK_SET);
	check(r == 0, "Set curson for file not succeded, return value of fseek: %d.", r);
	int c = 0;
	while((c = fgetc(flux)) != EOF) {
		if(c == '\n') ++ret;
	}
	return ret;
error:
	exit(EXIT_FAILURE);
}

char** getCommands(FILE* flux) {
	size_t size = nbCommands(flux);
	char** res = malloc(size * sizeof(char*));
	assert(flux != NULL);
	int r = 0, cour = 0, dec = 0, i = 0, index = 0;
	r = fseek(flux, 0, SEEK_SET);
	check(r == 0, "Set cursor for file nor succeded, return value of fseek: %d", r);
	while((i = fgetc(flux)) != EOF) {
		if(i != '\n') {
			++dec;
		} else {
			r = fseek(flux, cour, SEEK_SET);
			assert(r == 0);
			res[index] = malloc((dec+1) * sizeof(char));
			check_mem(fgets(res[index], dec+1, flux));
			fgetc(flux); // \n pas besoin de le prendre
			cour += dec+1;
			dec = 0;
			index++;
		}
	}
	return res;
error:
	exit(EXIT_FAILURE);
}

void close_file(FILE* fic) {
	if(fclose(fic) != 0) {
		perror("Probleme dans la fermeture de fichier");
		exit(EXIT_FAILURE);
	}
}

void free_commands(char** cmds, size_t s) {
	for(size_t i = 0; i < s; ++i) {
		free(cmds[i]);
	}
	free(cmds);
}
