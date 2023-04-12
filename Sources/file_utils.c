#include "../Headers/file_utils.h";
#include <stdio.h>
#include <stdlib.h>


size_t nbCommands(char* nomFic) {
	size_t ret = 0;
	FILE* fic = fopen(nomFic, "r");
	if(fic == NULL) {
		perror("Probleme dans l'ouverture de fichier");
		printf("%s\n", nomFic);
		exit(EXIT_FAILURE);
	} else {
		int c = 0;
		while((c = fgetc(fic)) != EOF) {
			if(c == '\n') ++ret;
		}
	}
	if(fclose(fic) != 0) {
		perror("Probleme dans la fermeture de fichier");
	}
	return ret;
}

char** getCommands(char* nomFic) {
	size_t size = nbCommands(nomFic);
    FILE* flux = fopen(nomFic, "r");
	char** res = malloc(size * sizeof(char));
	assert(flux != NULL);
	int r = 0, cour = 0, dec = 0, i = 0, index = 0;
	while((i = fgetc(flux)) != EOF) {
		if(i != '\n') {
			++dec;
		} else {
			r = fseek(flux, cour, SEEK_SET);
			assert(r == 0);
			res[index] = malloc((dec+2) * sizeof(char));
			char* st2 = fgets(res[index], dec+2, flux);
			assert(st2 != NULL);
			cour += dec+1;
			dec = 0;
			index++;
		}
	}
	r = fclose(flux);
	if(r != 0) {
		perror("Probleme dans la fermeture de fichier");
	}
	return res;
}
