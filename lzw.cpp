#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lzw.h"

DicoDecode tabDecode[TAILLE_DICO];
Cell *dico;
Cell *fin;

bool reste = false;
int bits_restants;


void initDico() {
    int i;
    Cell *node;
    for (i = 0; i < 256; i++) {
        node = (Cell *)malloc(sizeof(Cell));
        node->prefixe = -1;
        node->lettre = i;
        appendCell(node);
    }
}


void appendCell(Cell *node) {
    if (dico != NULL){
       fin->next = node;
     }
    else dico = node;
    fin = node;
    node->next = NULL;
}


void supprime_dico() {
    while (dico != NULL) {
        dico = dico->next;
    }
}


int dico_appartient(int prefixe, int lettre) {
    Cell *node;
    for (node = dico; node != NULL; node = node->next) {
        if (node->prefixe == prefixe && node->lettre == lettre) return node->val;
    }
    return -1;
}



void dico_ajoute(int prefixe, int lettre, int val) {
    Cell *node;
    node = (Cell *)malloc(sizeof(Cell));
    node->val = val;
    node->prefixe = prefixe;
    node->lettre = lettre;

    appendCell(node);
}



void ecriture(FILE * output, int code) {
    if (reste == true) {
        int code_precedent = (bits_restants << 4) + (code >> 8);
        fputc(code_precedent, output);
        fputc(code, output);

        reste = false;
    } else {
        bits_restants = code & 0xF;
        reste = true;

        fputc(code >> 4, output);
    }
}

int lecture(FILE * input) {
    int code = fgetc(input);
    if (code == EOF) return 0;

    if (reste == true) {
        code = (bits_restants << 8) + code;

        reste = false;
    } else {
        int code_suivant = fgetc(input);

        bits_restants = code_suivant & 0xF;
        reste = true;

        code = (code << 4) + (code_suivant >> 4);
    }
    return code;
}


int decode(int code, FILE * sortie) {
    int lettre;
    int tmp;

    if (code > 255) {
        lettre = dicoDecode_lettre(code);
        tmp = decode(dicoDecode_prefixe(code), sortie);
    }
    else {
        lettre = code;
        tmp = code;
    }
    fputc(lettre, sortie);

    return tmp;
}

void dicoDecode_ajoute(int prefixe, int lettre, int val) {
    tabDecode[val].prefixe = prefixe;
    tabDecode[val].lettre = lettre;
}

int dicoDecode_prefixe(int val) {
    return tabDecode[val].prefixe;
}

int dicoDecode_lettre(int val) {
    return tabDecode[val].lettre;
}

void compression(FILE *entree, FILE *sortie) {
    int prefixe = fgetc(entree);
    if (prefixe == EOF) {
        return;
    }
    int lettre;
    int code_suivant;
    int index;
    code_suivant = 256;
    initDico();


    while ((lettre = fgetc(entree)) != EOF) {

        if ((index = dico_appartient(prefixe, lettre)) != -1){
           prefixe = index;

         }
        else {

            ecriture(sortie, prefixe);

            if (code_suivant < TAILLE_DICO){
               dico_ajoute(prefixe, lettre, code_suivant);
               ++code_suivant;
             }


            prefixe = lettre;
        }
    }

    ecriture(sortie, prefixe);

    if (reste == true){

     fputc(bits_restants << 4, sortie);

   }

    supprime_dico();
}


void decompression(FILE * entree, FILE * sortie) {

    int code_precedent;
    int code_actuel;
    int code_suivant = 256;

    int caractere;

    code_precedent = lecture(entree);
    if (code_precedent == 0) {
        return;
    }
    fputc(code_precedent, sortie);

    while ((code_actuel = lecture(entree)) > 0) {

        if (code_actuel >= code_suivant) {
            fputc(caractere = decode(code_precedent, sortie), sortie);


        } else caractere = decode(code_actuel, sortie);


        if (code_suivant < TAILLE_DICO){
          dicoDecode_ajoute(code_precedent, caractere,code_suivant);
          ++code_suivant;
      }

        code_precedent = code_actuel;
    }

}
