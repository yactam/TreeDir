#include <stdio.h>
#include <assert.h>
#include "../Headers/liste_utils.h"
#include "../Headers/string_utils.h"

void liste(liste_noeud* l) {
	noeud* tmp = l;
	while(tmp != NULL) {
		printf("%s\n", tmp->nom);
		tmp = tmp->succ;
	}
}

noeud* create_liste(noeud* n) {
	liste_noeud* l = malloc(sizeof(liste_noeud*));
	assert(l != NULL);
	l->no = n;
	l->succ = NULL;
	return l;
}

noeud* add_liste(liste_noeud* l, noeud* n) {
	assert(l != NULL);
	noeud* tmp = l->no;
	while(tmp->succ != NULL) {
		tmp = tmp->succ;
	}
	tmp->succ = malloc(sizeof(noeud*));
	tmp->succ = n;
	return l;
}



