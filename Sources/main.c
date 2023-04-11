#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../Headers/commands.h"
#include "../Headers/string_utils.h"
#define N 1000

int main(int argc, char** argv) {
	printf("Simulation mini linux\n");
	noeud* n = init_program();
	while(true) {
		char* s = malloc(N * sizeof(char));
		printf("> ");
		fgets(s, N, stdin);
		parser* p = init(s, ' ');
		char* cmd = next(p);
		if(strcmp(cmd, "pwd\n") == 0) {
			pwd(n);
		} else if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "cd\n") == 0) {
			if(!hasNext(p)) n = cd_racine(n);
			else {
				char* aux = next(p);
				char* arg = malloc(strlen(aux) * sizeof(char));
				assert(arg != NULL);
				assert(memcpy(arg, aux, (strlen(aux)-1) * sizeof(char)) != NULL);
				printf("'%s'\n", arg);
				if(strcmp(arg, "..") == 0) n = cd_parent(n);
				else n = cd(n, arg, NULL);
			}
		} else if (strcmp(cmd, "ls\n") == 0 || strcmp(cmd, "ls") == 0) {
			ls(n);
		} else if (strcmp(cmd, "mkdir") == 0) {
			char* aux = next(p);
			char* arg = malloc(strlen(aux) * sizeof(char));
			assert(arg != NULL);
			assert(memcpy(arg, aux, (strlen(aux)-1) * sizeof(char)) != NULL);
			n = mkdir(n, arg);
		} else if(strcmp(cmd, "touch") == 0) {
			char* aux = next(p);
			char* arg = malloc(strlen(aux) * sizeof(char));
			assert(arg != NULL);
			assert(memcpy(arg, aux, (strlen(aux)-1) * sizeof(char)) != NULL);
			n = touch(n, arg);
		} else if(strcmp(cmd, "exit\n") == 0) {
			free_parser(p);
			free(s);
			// free program !!
			exit(0);
		} else {
			printf("Commande '%s' inconnue!\n", cmd);
			printf("avec ls %d\n", strcmp(cmd, "ls\n")); 
		}
		free(s);
		free_parser(p);
		// free program (rm /)
	}
    return EXIT_SUCCESS;
}
