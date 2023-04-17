#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "../Headers/string_utils.h"
#include "../Headers/liste_utils.h"
#include "../Headers/commands.h"
#define DEBUG 0

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
	assert(chem != NULL && n != NULL && strcmp(chem, "/") != 0);
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

/*********************************/
noeud* rm(noeud* n, char* chem) {
	assert(chem != NULL);
	char* target = last(chem, '/');
	if(DEBUG) printf("target: %s\n", target);
	size_t s = strlen(chem) - strlen(target); 
	if(DEBUG) printf("longuer du chemin(path): %zu\n", s);
	char* path = malloc(s * sizeof(char));
	assert(path != NULL);
	if(s > 0) assert(strncpy(path, chem, s) != NULL);
	*(path+s) = '\0';
	if(DEBUG) printf("path: %s\n", path);
	noeud* tmp = n;
	if(!estvide(path)) {
		if(strcmp(path, "/") == 0) tmp = cd_racine(tmp); 
		else tmp = cd(tmp, path);
		assert(tmp != NULL);
	}
	noeud* to_rm = find_liste(tmp->fils, target);
	if(DEBUG) {
		printf("à supprimer: %s, noeud courant: ", to_rm->nom);
		pwd(n);
		printf("est_parent: %d\n", est_parent(to_rm, n));
	}
	if(!estvide(path) && est_parent(to_rm, n)) {
		perror("Erreure rm le noeud corrant est inclut dans le sous arbre que vous voulez supprimer");
		exit(EXIT_FAILURE);
	}
	for(liste_noeud* l = to_rm->fils; l != NULL; l = l->succ) {
		rm(to_rm, l->no->nom);
	}
	assert(to_rm->fils == NULL);
	tmp->fils = remove_liste(tmp->fils, to_rm);
	assert(tmp->fils == NULL || find_liste(tmp->fils, to_rm->nom) == NULL);
	free(to_rm);
	tmp = n;
	return tmp;
}

/************** cp ********************/
noeud* cp(noeud* n, char* chem1, char* chem2) {
	char* from = last(chem1, '/');
	size_t s = strlen(chem1) - strlen(from);
	char* src = malloc((s+1) * sizeof(char));
	assert(src != NULL);
	if(s > 0) assert(strncpy(src, chem1, s) != NULL);
	*(src+s) = '\0';
	noeud* tmp = n;
	if(!estvide(src)) {
		if(strcmp(src, "/") == 0) tmp = cd_racine(tmp);
		else tmp = cd(tmp, src);
		assert(tmp != NULL);
	}
	noeud* to_cp = find_liste(tmp->fils, from);

	// Move to the destination
	char* to = last(chem2, '/');
	size_t t = strlen(chem2) - strlen(to);
	char* dst = malloc((t+1) * sizeof(char));
	assert(dst != NULL);
	if(t > 0) assert(strncpy(dst, chem2, t) != NULL);
	*(dst+t) = '\0';
	noeud* aux = n;
	if(!estvide(dst)) {
		if(strcmp(dst, "/") == 0) aux = cd_racine(aux);
		else aux = cd(aux, dst);
		assert(aux != NULL);
	}
	
	if(!aux->est_un_dossier) {
		perror("Erreure cp: la destination n'est pas un dossier");
		exit(EXIT_FAILURE);
	}
	if(find_liste(aux->fils, to) != NULL) {
		perror("Erreure cp: il y a un noeud avec le meme nom dans destionation");
		exit(EXIT_FAILURE);
	}
	if(est_parent(to_cp, aux)) {
		perror("Erreure cp: le noeud que vous voulez copier est inclut dans la destination (copie recursive sans fin)");
		exit(EXIT_FAILURE);
	}

	noeud* cp_noeud = malloc(sizeof(noeud));
	cp_noeud->est_un_dossier = to_cp->est_un_dossier;
	assert(memcpy(cp_noeud->nom, to, (strlen(to) + 1) * sizeof(char)) != NULL);
	cp_noeud->racine = to_cp->racine;
	cp_noeud->pere = aux;
	aux->fils = add_liste(aux->fils, cp_noeud);
	for(liste_noeud* l = to_cp->fils; l != NULL; l = l->succ) {
		size_t s1 = strlen(chem1), sn = strlen(l->no->nom);
		char* c1 = malloc((s1 + sn + 2) * sizeof(char));
		assert(memcpy(c1, chem1, s1 * sizeof(char)) != NULL);
		*(c1+s1) = '/';
		*(c1+s1+1) = '\0';
		assert(strcat(c1, l->no->nom) != NULL);
		*(c1 + s1 + sn + 1) = '\0';
		
		size_t s2 = strlen(chem2);
		char* c2 = malloc((s2 + sn + 2) * sizeof(char));
		assert(memcpy(c2, chem2, s2 * sizeof(char)) != NULL);
		*(c2+s2) = '/';
		*(c2+s2+1) = '\0';
		assert(strcat(c2, l->no->nom) != NULL);
		*(c2 + s2 + sn + 1) = '\0';
		n = cp(n, c1, c2);
	}
	return n;
}

/**********************************/
noeud* mv(noeud* n, char* chem1, char* chem2) {
	n = cp(n, chem1, chem2);
	n = rm(n, chem1);
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

/**********************************/
void tree_helper(noeud* n, char* padding) {
	if(n == NULL) return;
	if(n->est_un_dossier) printf(BLUE BOLD "%s%s\n" NORM DEFAULT, padding,  (n->pere == n) ? "/" : n->nom);
	else printf("%s%s\n", padding, n->nom);
	size_t s = strlen(padding) + 3;
	char* newPadding = malloc(s * sizeof(char));
	for(size_t i = 0; i < s; ++i) {
		*(newPadding+i) = ' ';
	}
	for(liste_noeud* f = n->fils; f != NULL; f = f->succ) {
		tree_helper(f->no, newPadding);
	}
}

void tree(noeud* n) {
	tree_helper(n, "");
}

