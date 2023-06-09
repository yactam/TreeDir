#ifndef __LISTE_UTILS_H__
#define __LISTE_UTILS_H__
#include "commands.h"
#include <stddef.h>
#define BOLD "\e[1m"
#define NORM "\e[m"
#define BLUE "\033[0;34m"
#define DEFAULT "\033[0m"

liste_noeud* create_liste(noeud*);
void liste(liste_noeud*);
liste_noeud* add_liste(liste_noeud*, noeud*);
liste_noeud* remove_liste(liste_noeud*, noeud*);
liste_noeud* rename_liste(liste_noeud*, noeud*, char[100]);
noeud* find_liste(liste_noeud*, char*);
void free_liste(liste_noeud*);
liste_noeud* copie_liste(liste_noeud*);
bool est_parent(noeud*, noeud*);
void print_fils(liste_noeud*);
size_t liste_size(liste_noeud*);

#endif
