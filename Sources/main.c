#include <stdlib.h>
#include <stdio.h>
#include "../Headers/commands.h"
#include "../Headers/string_utils.h"

int main(int argc, char** argv) {
	char* s = "Cours/ProjetC/td1/";
	parser* p = init(s, '/');
	while(hasNext(p)) {
		printf("%s\n", next(p));
	}
	free_parser(p);
    return EXIT_SUCCESS;
}
