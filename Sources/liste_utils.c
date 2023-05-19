#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wchar.h>
#include "../Headers/liste_utils.h"
#include "../Headers/string_utils.h"
#include "../Headers/debug.h"

void liste(liste_noeud* l) {
	liste_noeud* tmp = l;
	while(tmp != NULL) {
		if(tmp->no->est_un_dossier) printf(BLUE BOLD "%s\n" NORM DEFAULT, tmp->no->nom);
		else printf("%s\n", tmp->no->nom);
		tmp = tmp->succ;
	}
}

liste_noeud* create_liste(noeud* n) {
	debug("create new liste for node %s to start a new list of children for %s", n->nom, n->pere->nom);
	liste_noeud* l = malloc(sizeof(liste_noeud));
	check_mem(l);
	l->no = n;
	l->succ = NULL;
	return l;
error:
	log_error("failed to allocate memory to create liste with node %s", n->nom);
	return NULL;
}

liste_noeud* add_liste(liste_noeud* l, noeud* n) {
	debug("try to add node %s to %s children", (n != NULL ? n->nom : NULL), (n != NULL ? n->pere->nom : NULL)); 
	if(l == NULL) {
		l = create_liste(n);
		return l;
	}
	liste_noeud* tmp = l;
	while(tmp->succ != NULL) {
		check(strcmp(tmp->no->nom, n->nom) != 0,\
				"the node with name %s already exists in the list of children of %s.", n->nom, n->pere->nom)
		tmp = tmp->succ;
	}
	tmp->succ = malloc(sizeof(liste_noeud));
	check_mem(tmp->succ);
	tmp->succ->no = n;
	tmp->succ->succ = NULL;
	return l;
error:
	if(n) free_program(n->racine);
	exit(1);
}

liste_noeud* remove_liste(liste_noeud* l, noeud* n) { 
	if(l != NULL && n != NULL) {
		liste_noeud* s = l;
		if(l->no == n) {
			l = l->succ;
			free(s);
			return l;
		}
		liste_noeud* prev = NULL;
		while (s && s->no != n) {
			prev = s;
			s = s->succ;
		}
		if(s == NULL) return l;
		prev->succ = prev->succ->succ;
		free(s);
	}
	return l;
}

liste_noeud* rename_liste(liste_noeud* l, noeud* n, char name[100]) { // TODO
	return l;
}

noeud* find_liste(liste_noeud* l, char* nom) {
	if(l == NULL) return NULL;
	assert(l != NULL && nom != NULL);
	liste_noeud* tmp = l;
	while(tmp != NULL) {
		if(strcmp(tmp->no->nom, nom) == 0) return tmp->no;
		tmp = tmp->succ;
	}
	return NULL;
}

liste_noeud* copie_liste(liste_noeud* l) {
	if(l == NULL) return NULL;

	liste_noeud* h = create_liste(l->no);
	h->succ = copie_liste(l->succ);
	return h;
}

bool est_parent(noeud * n, noeud * m) { // Teste si m est un sous noeud de n
	noeud* tmp = m;
	if(n == m) return true;
	while(tmp->pere != tmp && tmp->pere != n) {
		tmp = tmp->pere;
	}
	if(tmp->pere == n) return true;
	return false;
}

void free_liste(liste_noeud* l) {
	liste_noeud* tmp = l;
	while(tmp != NULL) {
		tmp = tmp->succ;
		free(l->no);
		free(l);
		l = tmp;
	}
}

void print_fils(liste_noeud* l) {
	assert(l != NULL);
	liste_noeud* tmp = l;
	while(tmp->succ != NULL) {
		printf("%s %s, ", tmp->no->nom, (tmp->no->est_un_dossier ? "(D)" : "(F)"));
		tmp = tmp->succ;
	}
	printf("%s %s\n", tmp->no->nom, (tmp->no->est_un_dossier ? "(D)" : "(F)"));
}

size_t liste_size(liste_noeud* l) {
	size_t ret = 0;
	liste_noeud* tmp = l;
	while(tmp != NULL) {
		tmp = tmp->succ;
		++ret;
	}
	return ret;
}
