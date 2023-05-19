#include <string.h>
#include <stdbool.h>


struct parser {
	char* buffer;
	int next; // The index of the starting of the next world
	char delimiter;
};

typedef struct parser parser;

parser* init(char*, char);
void free_parser(parser*);
char* next(parser*);
void free_word(char*);
bool hasNext(parser*);
bool isalphanum(char*);
bool estvide(char*);
char *last(char *, char);
