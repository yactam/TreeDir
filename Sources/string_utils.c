#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Headers/string_utils.h"

parser* init(char* string, char delimiter) {
	parser* p = malloc(sizeof(parser));
	assert(p != NULL);
	size_t len = strlen(string);
	p->buffer = malloc(sizeof(len));
	assert(p->buffer != NULL);
	assert(memcpy(p->buffer, string, len * sizeof(char)) != NULL);
	p->delimiter = delimiter;
	p->next = 0;
	if(*string == delimiter) p->next += 1;
	p->tmp = malloc(sizeof(char));
	assert(p->tmp != NULL);
	return p;
}

void free_parser(parser* p) {
	free(p->buffer);
	free(p->tmp);
	free(p);
}

bool hasNext(parser* p) {
	return p->next != -1;
}

char* next(parser* p) {
	assert(p->next != -1);
	size_t l = 0;
	for(size_t i = p->next; i < strlen(p->buffer) && *(p->buffer+i) != p->delimiter; ++i) ++l;
	char* ret = malloc(l * sizeof(char));
	if(l != 0) {
		assert(ret != NULL);
		assert(memcpy(ret, p->buffer+p->next, l * sizeof(char)) != NULL);
		free(p->tmp);
		p->tmp = ret;
	} else {
		ret = ""; // Pour signaler une erreure dans le chemin par ex: /Cours////ProjetC (plusieurs <</>> consecutifs)
	}
	p->next += l+1;
	if(p->next >= strlen(p->buffer)) p->next = -1;
	return ret;
}

bool isalphanum(char* s) {
	for(size_t i = 0; i < strlen(s); ++i) {
		if(!isalnum(s[i])) return false;
	}
	return true;
}

bool estvide(char* s) {
	return strcmp(s, "") == 0;
}

char *last(char *s, char d){
	int len = strlen(s);
	int index;
	char *end = s+len-1;
	while (*end != d){
		end--;
	}
	char *res = malloc(strlen(end));
	strcpy(res, end+1);
	return res;
}