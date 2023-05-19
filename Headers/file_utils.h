#ifndef _LISTE_UTILSH_
#define _LISTE_UTILSH_

#include <stddef.h>
#include <stdio.h>

FILE* file_reader(char*);
size_t nbCommands(FILE*);
char** getCommands(FILE*);
void close_file(FILE*);
void free_commands(char**, size_t);

#endif
