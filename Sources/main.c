#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../Headers/commands.h"
#include "../Headers/string_utils.h"
#include "../Headers/file_utils.h"
#define SLEEP_TIME 1000000

int main(int argc, char** argv) {
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
			} else if(strcmp(cmd, "cp") == 0) {
				char* src = next(p);
				char* dst = next(p);
				n = cp(n, "/Cours", dst);
			} else if(strcmp(cmd, "rm") == 0) {
				char* aux = next(p);
				n = rm(n, aux);
			} else if(strcmp(cmd, "print") == 0) {
				print(n);
			} else if(strcmp(cmd, "tree") == 0) {
				tree(n);
			} else {
				printf("Commande '%s' inconnue!\n", cmd); 
			}
			free_parser(p);
			usleep(SLEEP_TIME);
		}
		close_file(fic);
		free(cmds);
	}
	return EXIT_SUCCESS;
}
