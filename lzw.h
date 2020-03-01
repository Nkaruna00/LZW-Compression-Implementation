#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cell{

    int val;
    int prefixe;
    int lettre;
    struct Cell *next;
};

typedef struct Cell Cell;


struct DicoDecode{
    int prefixe;
    int lettre;
};
typedef struct DicoDecode DicoDecode;


#define TAILLE_DICO 4095


void initDico();
void appendCell(Cell *node);
void supprime_dico();
int dico_appartient(int prefixe, int lettre);
int dicoprefixe(int val);
int dicolettre(int val);
void dico_ajoute(int prefixe, int lettre, int val);

void ecriture(FILE * sortie, int code);
int lecture(FILE * entree);

void compression(FILE *entree, FILE *sortie);
void decompression(FILE *entree, FILE *sortie);
int decode(int code, FILE * sortie);

void dicoDecode_ajoute(int prefixe, int lettre, int val);
int dicoDecode_prefixe(int val);
int dicoDecode_lettre(int val);
