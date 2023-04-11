#include <stdbool.h>
#ifndef _COMMANDSH_
#define _COMMANDSH_

#define ERREUR_CHEMIN_INEXSISTANT 1
#define ERREUR_CHEMIN_INVALIDE 2
#define ERREUR_CONTIENT_FICHIER 3

#define hasEror(n) (n == 1  || n == 2  || n==3)

// Definition de la structure principale qui encapsule un arbre représentant l'organisation des dossiers/fichiers 

struct noeud {
    bool est_un_dossier;
    char nom[100];
    struct noeud* pere;
    struct noeud* racine;
    struct liste_noeud* fils;
};

struct liste_noeud {
    struct noeud* no; // noeud actuel 
    struct liste_noeud* succ; // son successeur
};

typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;

// Definition des prototypes des fonctions représentant les instructions qui manipule la structure de données

noeud* init_program();
void ls(noeud*);
void pwd(noeud*);
noeud* cd(noeud*, char*, int*);
noeud* cd_parent(noeud*);
noeud* cd_racine(noeud*);
noeud* mkdir(noeud*, char*);
noeud* touch(noeud*, char*);
noeud* rm(noeud*, char*);
noeud* cp(noeud*, char*, char*);
noeud* mv(noeud*, char*, char*);
void print(noeud*);

#endif
