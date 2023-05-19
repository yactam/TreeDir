#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "../Headers/string_utils.h"
#include "../Headers/liste_utils.h"
#include "../Headers/commands.h"
#include "../Headers/debug.h"

noeud* init_program() {
	debug("initialisation of the program.");
	noeud* root = malloc(sizeof(noeud));
	check_mem(root);
	root->est_un_dossier = true;
	check(strcpy(root->nom, "") != NULL, "name root \"\" failed!");
	root->pere = root;
	root->racine = root;
	root->fils = NULL;
	return root;
error:
	free_program(root);
	return NULL;
}

/**********************************/
void ls(noeud* n) {
	debug("call to liste all children of node '%s'", (n != NULL ? n->nom : NULL));
	if(n != NULL) {
		liste(n->fils);
	} else {
		log_error("the node in the call for ls is null.");
	}
}

/**********************************/
void pwd_helper(noeud* n, noeud* end) {
	if(n->pere == n) { // La racine
		(n != end) ? printf("/") : printf("/\n");
	} else {
		pwd_helper(n->pere, n);
		n != end ? printf("%s/", n->nom) : printf("%s\n", n->nom);
	}
}

void pwd(noeud* n) {
	debug("call to pwd in the node '%s'", (n != NULL ? n->nom : NULL));
	if(n != NULL) {
		pwd_helper(n, n);
	} else {
		log_error("the node in the call for pwd is null.");
	}
}

/**********************************/
noeud* cd(noeud* n, char* chem) {
	debug("call to change directory from '%s' with path: '%s'", n->nom, chem);
	check(chem != NULL && n != NULL && strcmp(chem, "/") != 0 && !estvide(chem), "the arguments of cd are incorrecte.");
	parser* p = init(chem, '/');
	noeud* ret = n;
	char* s = NULL;
	if(*chem == '/') ret = n->racine;
	while(hasNext(p)) {
		s = next(p);
		check(!estvide(s), "error in the path for cd: '%s'.", chem);
		if(strcmp(s, ".") == 0) {
			ret = ret;
		} else if(strcmp(s, "..") == 0 ) {
			ret = cd_parent(ret);
		} else {
			noeud* vers = find_liste(ret->fils, s);
			check(vers != NULL, "the node '%s' in the path for cd doesn't exist.", s);
			check(vers->est_un_dossier, "the node '%s' in path for the command cd is not a folder.", vers->nom);
			ret = vers;
		}
		free_word(s);
	}
	free_parser(p);
	return ret;
error:
	if(p) free_parser(p);
	if(s) free_word(s);
	if(n) free_program(n->racine);
	exit(1);
}

/**********************************/
// Pour la commande cd ..
noeud* cd_parent(noeud* n) {
	debug("call to cd to parent of node '%s'.", (n != NULL ? n->nom : NULL));
	assert(n != NULL);
	return n->pere;
}

/**********************************/
// Pour la commande cd
noeud* cd_racine(noeud* n) {
	debug("call to cd to the root with node: '%s'", (n != NULL ? n->nom : NULL));
	assert(n != NULL);
	return n->racine;
}

noeud* init_noeud() {
	noeud* n = malloc(sizeof(noeud));
	assert(n != NULL);
	n->est_un_dossier = false;
	n->fils = NULL;
	n->pere = NULL;
	n->racine = NULL;
	n->nom[0] = '\0';
	return n;
}

/*********************************/
noeud* add_noeud(noeud* n, char* nom, bool isDir) {
	size_t len = strlen(nom);
	check(len != 0 && len <= 99 && isalphanum(nom), "the name to add node is invalid: '%s'.", nom);
	noeud* f = init_noeud();
	f->est_un_dossier = isDir;
	check_mem(memcpy(f->nom, nom, len * sizeof(char)));
	f->nom[len] = '\0';
	f->pere = n;
	f->racine = n->racine;
	n->fils = add_liste(n->fils, f);
	return n;
error:
	if(n) free_program(n->racine);
	exit(1);
}

noeud* mkdir(noeud* n, char* nom) {
	debug("call to make directory in the node '%s' with name '%s'.", (n != NULL ? n->nom : NULL), nom); 
	return add_noeud(n, nom, true);
}

noeud* touch(noeud* n, char* nom) {
	debug("call to touch file in the node '%s' with name '%s'.", (n != NULL ? n->nom : NULL), nom); 
	return add_noeud(n, nom, false);
}

/*********************************/
noeud* find_target_parent(noeud* n, char* chem, char* target) {
	size_t s = strlen(chem) - strlen(target);
	char* path = malloc((s+1) * sizeof(char));
	check_mem(path);
	if(s > 0) check_mem(strncpy(path, chem, s));
	*(path+s) = '\0';
	noeud* tmp = n;
	if(!estvide(path)) {
		if(strcmp(path, "/") == 0) tmp = cd_racine(tmp);
		else tmp = cd(tmp, path);
	}
	free(path);
	check_mem(tmp);
	return tmp;
error:
	if(n) free_program(n->racine);
	exit(EXIT_FAILURE);
}

noeud* rm(noeud* n, char* chem) {
	debug("call to rm from node '%s' with path: '%s'.", (n != NULL ? n->nom : NULL), chem);
	check_mem(chem);
	char* target = last(chem, '/');
	debug("target to rm: '%s'.", target);
	noeud* tmp = find_target_parent(n, chem, target);
	noeud* to_rm = find_liste(tmp->fils, target);
	debug("to remove: '%s', actual node: '%s'.", to_rm->nom, n->nom);
	check(to_rm, "there is no node with name '%s' in node '%s'", to_rm->nom, tmp->nom);
	check(!est_parent(to_rm, n), "the current node is included in the sub tree that will be deleted.");
	for(liste_noeud* l = to_rm->fils; l != NULL; l = to_rm->fils) {
		rm(to_rm, l->no->nom);
	}
	check(to_rm->fils == NULL, "the children of node '%s' need to be deleted.", to_rm->nom);
	tmp->fils = remove_liste(tmp->fils, to_rm);
	check(tmp->fils == NULL || find_liste(tmp->fils, to_rm->nom) == NULL, "the child '%s' was not deleted from '%s'.", to_rm->nom, tmp->nom);
	free(to_rm);
	free(target);
	tmp = n;
	return tmp;
error:
	if(target) free(target);
	if(n) free_program(n->racine);
	exit(EXIT_FAILURE);
}

/************** cp ********************/
noeud* cp(noeud* n, char* chem1, char* chem2) {
	debug("call to cp in node '%s' from '%s' to '%s'.", (n != NULL ? n->nom : NULL), chem1, chem2);
	char* from = last(chem1, '/');
	debug("source to cp name: '%s'.", from);
	noeud* tmp = find_target_parent(n, chem1, from);
	debug("parent folder of the source file: '%s'", tmp->nom);
	noeud* to_cp = find_liste(tmp->fils, from);
	debug("to copy from node: '%s'", to_cp->nom);
	char* to = last(chem2, '/');
	noeud* aux = find_target_parent(n, chem2, to);
	debug("destination node to copy: '%s'", aux->nom);

	check(aux->est_un_dossier, "the destination node for cp('%s') is not a directory.", aux->nom);
	check(find_liste(aux->fils, to) == NULL, "there is a node with the same name as '%s' in destination node.", to);
	check(!est_parent(to_cp, aux), "the node that you want to copy ('%s') is included in the destination ('%s').", to_cp->nom, aux->nom);

	noeud* cp_noeud = init_noeud();
	cp_noeud->est_un_dossier = to_cp->est_un_dossier;
	check_mem(strcpy(cp_noeud->nom, to));
	cp_noeud->racine = to_cp->racine;
	cp_noeud->pere = aux;
	aux->fils = add_liste(aux->fils, cp_noeud);

	for(liste_noeud* l = to_cp->fils; l != NULL; l = l->succ) {
		size_t s1 = strlen(chem1), sn = strlen(l->no->nom);
		char* c1 = malloc((s1 + sn + 2) * sizeof(char));
		check_mem(memcpy(c1, chem1, s1 * sizeof(char)));
		*(c1+s1) = '/';
		*(c1+s1+1) = '\0';
		check_mem(strcat(c1, l->no->nom));
		*(c1 + s1 + sn + 1) = '\0';
		
		size_t s2 = strlen(chem2);
		char* c2 = malloc((s2 + sn + 2) * sizeof(char));
		check_mem(memcpy(c2, chem2, s2 * sizeof(char)));
		*(c2+s2) = '/';
		*(c2+s2+1) = '\0';
		check_mem(strcat(c2, l->no->nom));
		*(c2 + s2 + sn + 1) = '\0';
		n = cp(n, c1, c2);
		free(c1);
		free(c2);
	}
	free(from);
	free(to);
	return n;
error:
	if(n) free_program(n->racine);
	exit(EXIT_FAILURE);
}

/**********************************/
noeud* mv(noeud* n, char* chem1, char* chem2) {
	debug("call to move in node '%s' from '%s' to '%s'.", (n != NULL ? n->nom : NULL), chem1, chem2);
	char* from = last(chem1, '/');
	debug("source to move name: '%s'.", from);
	noeud* tmp = find_target_parent(n, chem1, from);
	debug("parent folder of the source file: '%s'", tmp->nom);
	noeud* to_mv = find_liste(tmp->fils, from);
	debug("to move from node: '%s'", to_mv->nom);
	char* to = last(chem2, '/');
	noeud* aux = find_target_parent(n, chem2, to);
	debug("destination node to move to: '%s'", aux->nom);

	check(aux->est_un_dossier, "the destination node for mv('%s') is not a directory.", aux->nom);
	check(find_liste(aux->fils, to) == NULL, "there is a node with the same name as '%s' in destination node.", to);
	check(!est_parent(to_mv, aux), "the node that you want to copy ('%s') is included in the destination ('%s').", to_mv->nom, aux->nom);
	
	tmp->fils = remove_liste(tmp->fils, to_mv);
	check_mem(strcpy(to_mv->nom, to));
	to_mv->pere = aux;
	aux->fils = add_liste(aux->fils, to_mv);

	return n;
error:
	if(n) free_program(n->racine);
	exit(EXIT_FAILURE);
}

/**********************************/
void print(noeud* n) {
	debug("call to print with node: '%s'", (n->pere == n) ? "/" : n->nom);
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
	char* newPadding = malloc((s+1) * sizeof(char));
	for(size_t i = 0; i < s; ++i) {
		*(newPadding+i) = ' ';
	}
	*(newPadding+s) = '\0';
	for(liste_noeud* f = n->fils; f != NULL; f = f->succ) {
		tree_helper(f->no, newPadding);
	}
	free(newPadding);
}

void tree(noeud* n) {
	debug("call to tree with node: '%s'.", (n->pere == n) ? "/" : n->nom);
	tree_helper(n, "");
}
/**********************************/
void free_program(noeud* root) {
	debug("call to free the program from node: '%s'", (root != NULL ? root->nom : NULL)); 
	if(root) {
		root = root->racine;
		for(liste_noeud* f = root->fils; f != NULL; f = root->fils) {
			root = rm(root, f->no->nom);
		}
		free_liste(root->fils);
		free(root->fils);
		free(root);
	}
}
