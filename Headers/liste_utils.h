#ifndef _LISTE_UTILSH_
#define _LISTE_UTILSH_
#include "../Headers/commands.h"

liste_noeud* create_liste(noeud*);
void liste(liste_noeud*);
liste_noeud* add_liste(liste_noeud*, noeud*);
liste_noeud* remove_liste(liste_noeud*, noeud*);
liste_noeud* rename_liste(liste_noeud*, noeud*, char[100]);
noeud* find_liste(liste_noeud*, char*);
void free_liste(liste_noeud*);

#endif
