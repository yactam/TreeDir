#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../Headers/commands.h"
#include "../Headers/string_utils.h"
#include "../Headers/file_utils.h"
#include "../Headers/debug.h"
#define SLEEP_TIME 3
#define PURPLE "\033[0;35m" 
#define DEFAULT "\033[0m"

int main(int argc, char** argv) {
	check(argc == 2, "The program waits for one file as an argument, you have provided %d arguments.", argc-1);
	char* path = argv[1];
	assert(path != NULL);
	FILE* fic = file_reader(path);
	size_t s = nbCommands(fic);
	char** cmds = getCommands(fic);
	noeud* n = init_program();
	for(size_t i = 0; i < s; ++i) {
		char* ligne = cmds[i];
		assert(ligne != NULL);
		printf(PURPLE ">%s\n" DEFAULT, ligne);
		parser* p = init(ligne, ' ');
		char* cmd = next(p);
		if(strcmp(cmd, "pwd") == 0) {
			check(!hasNext(p), "too many arguments for command pwd.");
			pwd(n);
			free_word(cmd);
		} else if (strcmp(cmd, "cd") == 0) {
			if(!hasNext(p)) {
				n = cd_racine(n);
				free_word(cmd);
			}
			else {
				char* aux = next(p);
				check(!hasNext(p), "too many arguments for command cd.");
				if(strcmp(aux, "..") == 0) n = cd_parent(n);
				else n = cd(n, aux);
				free_word(cmd);
				free_word(aux);
			}
		} else if (strcmp(cmd, "ls") == 0) {
			check(!hasNext(p), "no arguments needed for command ls.");
			ls(n);
			free_word(cmd);
		} else if (strcmp(cmd, "mkdir") == 0) {
			char* aux = next(p);
			check(!hasNext(p), "too many arguments for command mkdir.");
			n = mkdir(n, aux);
			free_word(cmd);
			free_word(aux);
		} else if(strcmp(cmd, "touch") == 0) {
			char* aux = next(p);
			check(!hasNext(p), "too many arguments for command touch.");
			n = touch(n, aux);
			free_word(cmd);
			free_word(aux);
		} else if(strcmp(cmd, "cp") == 0) {
			char* src = next(p);
			char* dst = next(p);
			check(!hasNext(p), "too many arguments for command cp.");
			n = cp(n, src, dst);
			free_word(cmd);
			free_word(src);
			free_word(dst);
		} else if(strcmp(cmd, "mv") == 0) {
			char* src = next(p);
			char* dst = next(p);
			check(!hasNext(p), "too many arguments for command mv.");
			n = mv(n, src, dst);
			free_word(cmd);
			free_word(src);
			free_word(dst);
		}else if(strcmp(cmd, "rm") == 0) {
			char* aux = next(p);
			check(!hasNext(p), "too many arguments for command rm.");
			n = rm(n, aux);
			free_word(cmd);
			free_word(aux);
		} else if(strcmp(cmd, "print") == 0) {
			check(!hasNext(p), "too many arguments for command print.");
			print(n->racine);
			free_word(cmd);
		} else if(strcmp(cmd, "tree") == 0) {
			check(!hasNext(p), "too many arguments for command tree.");
			tree(n);
			free_word(cmd);
		} else {
			log_error("Commande '%s' inconnue!\n", cmd); 
			free_word(cmd);
		}
		free_parser(p);
		usleep(SLEEP_TIME);
	}
	close_file(fic);
	free_program(n->racine);
	free_commands(cmds, s);
	return EXIT_SUCCESS;

error:
	if(n) free_program(n);
	if(fic) close_file(fic);
	if(cmds) free_commands(cmds, s);
	return EXIT_FAILURE;
}
