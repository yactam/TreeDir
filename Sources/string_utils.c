#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Headers/string_utils.h"
#include "../Headers/debug.h"

parser* init(char* string, char delimiter) {
	parser* p = malloc(sizeof(parser));
	check_mem(p);
	size_t len = strlen(string);
	p->buffer = malloc((len+1) * sizeof(char));
	check_mem(p->buffer);
	check_mem(memcpy(p->buffer, string, len * sizeof(char)));
	*(p->buffer+len) = '\0';
	p->delimiter = delimiter;
	p->next = 0;
	if(*string == delimiter) p->next += 1;
	return p;
error:
	return NULL;
}

void free_parser(parser* p) {
	free(p->buffer);
	free(p);
}

bool hasNext(parser* p) {
	return p->next != -1;
}

char* next(parser* p) {
	check(p->next != -1, "parser has no next.");
	size_t l = 0;
	for(size_t i = p->next; i < strlen(p->buffer) && *(p->buffer+i) != p->delimiter; ++i) ++l; // La longueur de la chaine entre deux dilimiteur 
	char* ret = malloc((l+1) * sizeof(char));
	check_mem(ret != NULL);
	if(l != 0) {
		check_mem(memcpy(ret, p->buffer+p->next, l * sizeof(char)));
		*(ret+l) = '\0';
	} else {
		*ret = '\0'; // Pour signaler une erreure dans le chemin par ex: /Cours////ProjetC (plusieurs <</>> consecutifs)
	}
	p->next += l+1;
	if(p->next >= strlen(p->buffer)) p->next = -1;
	return ret;
error:
	return NULL;
}

void free_word(char* w) {
	if(w != NULL) free(w);
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

char* last(char *s, char d){
	assert(s != NULL);
	int len = strlen(s);
	char *end = s+len-1;
	while (end != s && *(end-1) != d){
		end--;
	}
	size_t n = strlen(end);
	char *res = malloc((n+1) * sizeof(char));
	check_mem(res);
	check_mem(strcpy(res, end));
	*(res+n) = '\0';
	return res;
error:
	return NULL;
}
