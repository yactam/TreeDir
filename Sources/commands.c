#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "../Headers/string_utils.h"
#include "../Headers/liste_utils.h"
#include "../Headers/commands.h"

noeud* init_program() {
	noeud* root = malloc(sizeof(noeud*));
	assert(root != NULL);
	root->est_un_dossier = true;
	assert(memmove(root->nom, "", sizeof(char)) != NULL);
	root->pere = root;
	root->racine = root;
	root->fils = NULL;
	return root;
}

/**********************************/
void ls(noeud* n) {
	if(n != NULL) liste(n->fils);
}

/**********************************/
void pwd_helper(noeud* n, noeud* end) {
	if(n->pere == n) {
		(n != end) ? printf("/") : printf("/\n");
	} else {
		pwd_helper(n->pere, n);
		n != end ? printf("%s/", n->nom) : printf("%s\n", n->nom);
	}
}

void pwd(noeud* n) {
	pwd_helper(n, n);
}
/**********************************/
noeud* cd(noeud* n, char* chem) {
	assert(chem != NULL && n != NULL);
	parser* p = init(chem, '/');
	noeud* ret = n;
	if(*chem == '/') ret = n->racine;
	while(hasNext(p)) {
		char* s = next(p);
		if(estvide(s)) {
			printf("Erreure dans le chemin %s!\n", s);
			return n;
		} else if(strcmp(s, ".") == 0) {
			ret = ret;
		} else if(strcmp(s, "..") == 0 ) {
			ret = cd_parent(ret);
		} else {
			noeud* vers = find_liste(ret->fils, s);
			if(vers == NULL) {
				printf("Le chemin n'existe pas!\n");
				return n;
			} else if(!vers->est_un_dossier) {
				printf("Le chemin est incorrect %s n'est pas un dossier!\n", vers->nom);	
			} else {
				ret = vers;
			}
		}
	}
	return ret;
}

/**********************************/
// Pour la commande cd ..
noeud* cd_parent(noeud* n) {
	assert(n != NULL);
	return n->pere;
}

/**********************************/
// Pour la commande cd
noeud* cd_racine(noeud* n) {
	assert(n != NULL);
	return n->racine;
}

/*********************************/
noeud* add_noeud(noeud* n, char* nom, bool isDir) {
	size_t len = strlen(nom);
	if(len == 0 || len > 99 || !isalphanum(nom)) {
		printf("Nom invalide!\n");
	} else {
		noeud* f = malloc(sizeof(noeud));
		f->est_un_dossier = isDir;
		assert(memcpy(f->nom, nom, len * sizeof(char)) != NULL);
		f->pere = n;
		f->racine = n->racine;
		if(n->fils == NULL) n->fils = create_liste(f);
		else n->fils = add_liste(n->fils, f);
	}
	return n;
}

noeud* mkdir(noeud* n, char* nom) {
	return add_noeud(n, nom, true);
}

noeud* touch(noeud* n, char* nom) {
	return add_noeud(n, nom, false);
}

/**********************************/

/**********************************/
