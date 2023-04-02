#include <string.h>
#include <stdbool.h>


struct parser {
	char* buffer;
	int next; // The index of the starting of the next world
	char delimiter;
	char* tmp;
};

typedef struct parser parser;

parser* init(char*, char);
void free_parser(parser*);
char* next(parser*);
bool hasNext(parser*);

