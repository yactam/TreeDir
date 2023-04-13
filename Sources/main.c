#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../Headers/commands.h"
#include "../Headers/string_utils.h"
#include "../Headers/file_utils.h"

int main(int argc, char** argv) {
	int debug = 0;
	if(argc != 2) {
		printf("Ce programme attends qu'un seul fichiers en parametre, vous avez fourni %d arguments\n", argc-1);
		exit(EXIT_FAILURE);
	} else {
		char* path = argv[1];
		assert(path != NULL);
		FILE* fic = file_reader(path);
		size_t s = nbCommands(fic);
		char** cmds = getCommands(fic);
		noeud* n = init_program();
		for(size_t i = 0; i < s; ++i) {
			char* ligne = cmds[i];
			printf(">%s\n", ligne);
			parser* p = init(ligne, ' ');
			char* cmd = next(p);
			if(strcmp(cmd, "pwd") == 0) {
				pwd(n);
			} else if (strcmp(cmd, "cd") == 0) {
				if(!hasNext(p)) {
					n = cd_racine(n);
				}
				else {
					char* aux = next(p);
					if(strcmp(aux, "..") == 0) n = cd_parent(n);
					else n = cd(n, aux);
				}
			} else if (strcmp(cmd, "ls") == 0) {
				ls(n);
			} else if (strcmp(cmd, "mkdir") == 0) {
				char* aux = next(p);
				n = mkdir(n, aux);
			} else if(strcmp(cmd, "touch") == 0) {
				char* aux = next(p);
				n = touch(n, aux);
			} else if(strcmp(cmd, "exit") == 0) {
				free_parser(p);
				// free program !!
				exit(0);
			} else if(strcmp(cmd, "print") == 0) {
				print(n);
			} else if(strcmp(cmd, "tree") == 0) {
				tree(n);
			} else {
				printf("Commande '%s' inconnue!\n", cmd); 
			}
			free_parser(p);
		}
		close_file(fic);
		free(cmds);
	}
	return EXIT_SUCCESS;
}
