#include <stdbool.h>

typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;

struct noeud {
	bool est_dossier;
	char nom[100];
	noeud* pere;
	noeud* racine;
	liste_noeud* fils;
}

struct liste_noeud {
	noeud* no;
	liste_noeud* succ;
}

void liste(liste_noeud*);
liste_neoud* add(noeud*);
liste_noeud* remove(noeud*);
liste_noeud* rename(noeud*, char[100]);
