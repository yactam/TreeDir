#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "../Headers/commands.h"
#include "../Headers/string_utils.h"
#include "../Headers/list_utils.h"

noeud* init() {
	noeud* root = malloc(sizeof(noeud*));
	assert(root != NULL);
	root->est_dossier = true;
	root->nom = "";
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
void pwd(noeud* n) {

}
/**********************************/
noeud* cd(noeud* n, char* c) {
	
}

/**********************************/
// Pour la commande cd ..
noeud* cd_parent(noeud* n) {
	assert(n != NULL);
	return n->pere;
}

/**********************************/
// Pour la commande cd
noeud* cd() {
	assert(n != NULL);
	return n->racine;
}

/*********************************/

