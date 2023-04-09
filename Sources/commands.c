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
noeud* cd(noeud* n, char* chem, int *erreur) {
	assert(chem != NULL && n != NULL);
	parser* p = init(chem, '/');
	noeud* ret = n;
	if(*chem == '/') ret = n->racine;
	while(hasNext(p)) {
		char* s = next(p);
		if(estvide(s)) {
			printf("Erreure dans le chemin %s!\n", s);
			if (erreur) *erreur = ERREUR_CHEMIN_INVALIDE;
			return n;
		} else if(strcmp(s, ".") == 0) {
			ret = ret;
		} else if(strcmp(s, "..") == 0 ) {
			ret = cd_parent(ret);
		} else {
			noeud* vers = find_liste(ret->fils, s);
			if(vers == NULL) {
				printf("Le chemin n'existe pas!\n");
				if (erreur) *erreur = ERREUR_CHEMIN_INEXSISTANT;
				return n;
			} else if(!vers->est_un_dossier) {
				printf("Le chemin est incorrect %s n'est pas un dossier!\n", vers->nom);
				if (erreur) *erreur = ERREUR_CONTIENT_FICHIER;	
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

/************** cp ********************/
bool est_inclut(noeud *n1, noeud *n2){
	while (n2){
		if (n2 == n1) return true;
		n2 = n2->pere;
	}
	return false;
}

void cp_aux(noeud *n, noeud *src, char *dst){
	char *lastW = last(dst, '/');
	char *pahtTruncated = malloc(strlen(dst) - strlen(lastW));
	strncpy(pahtTruncated, dst, strlen(dst) - strlen(lastW));
	int erreur = 0;
	noeud *dst_noeud = cd(n, pahtTruncated, &erreur);
	if (hasEror(erreur)) return;
	if (!dst_noeud->est_un_dossier){
		print("Erreur le chemin de destination n'est pas un dossier\n");
		return;
	}
	if (est_inclut(src, dst_noeud)){
		printf("Erreur %s est un sous arbre du repertoire %s\n", dst, src->nom);
		return;
	}
	if (find_liste(dst_noeud->fils, lastW)){
		printf("Erreur le nom %s exsiste deja\n");
		return;
	}
	if (src->pere) 
		src->pere->fils = remove_liste(src->pere->fils, src);
	strcpy(src->nom, lastW);
	dst_noeud->fils = add_liste(dst_noeud->fils, src);
}

noeud* cp(noeud* n, char* src, char* dst){
	int erreur = 0;
	assert(erreur);
	noeud *src_noeud = cd(n, src, &erreur);
	if (!hasEror(erreur))
		cp_aux(n, src_noeud, dst);
	free(erreur);
	return n;
}
/**********************************/
void print(noeud* n) {
	printf("Noeud %s ", (n->pere == n) ? "/" : n->nom);
	if(n->est_un_dossier) printf("(D), ");
	else printf("(F), ");
	if(n->pere != n) printf("pere: %s, ", (n->pere == n->racine) ? "/" : n->pere->nom);
	size_t s = liste_size(n->fils);
	printf("%zu fils%s", s, (s != 0 ? ": " : "\n"));
	if(n->fils != NULL) print_fils(n->fils);
	for(liste_noeud* f = n->fils; f != NULL; f = f->succ) {
		print(f->no);
	}
}

