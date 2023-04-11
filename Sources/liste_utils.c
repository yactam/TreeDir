#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wchar.h>
#include "../Headers/liste_utils.h"
#include "../Headers/string_utils.h"

void liste(liste_noeud* l) {
	liste_noeud* tmp = l;
	while(tmp != NULL) {
		printf("%s\n", tmp->no->nom);
		tmp = tmp->succ;
	}
}

liste_noeud* create_liste(noeud* n) {
	liste_noeud* l = malloc(sizeof(liste_noeud*));
	assert(l != NULL);
	l->no = n;
	l->succ = NULL;
	return l;
}

liste_noeud* add_liste(liste_noeud* l, noeud* n) {
	assert(l != NULL);
	liste_noeud* tmp = l;
	while(tmp->succ != NULL) {
		if(strcmp(tmp->no->nom, n->nom) == 0) { // Si le nom existe déja dans la liste on va pas encore l'ajouter une autre fois
			printf("Le nom %s existe déja!\n", n->nom);
			return l; // Peut-etre on peut meme faire exit 1 içi directement
		}
		tmp = tmp->succ;
	}
	tmp->succ = malloc(sizeof(liste_noeud*));
	assert(tmp->succ != NULL);
	tmp->succ->no = n;
	tmp->succ->succ = NULL;
	return l;
}

liste_noeud* remove_liste(liste_noeud* l, noeud* n) { // TODO
	liste_noeud *prec = NULL;
	liste_noeud *s = l;
	while (l){
		if (l->no == n){ 
			if (!prec) return l->succ;
			prec->succ = l->succ; 
		}
		prec = l;
		l = l->succ;
	}
	return s;
}

liste_noeud* rename_liste(liste_noeud* l, noeud* n, char name[100]) { // TODO
	return l;
}

noeud* find_liste(liste_noeud* l, char* nom) {
	assert(l != NULL && nom != NULL);
	liste_noeud* tmp = l;
	while(tmp != NULL) {
		if(strcmp(tmp->no->nom, nom) == 0) return tmp->no;
		tmp = tmp->succ;
	}
	return NULL;
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
