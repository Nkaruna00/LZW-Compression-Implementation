#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lzw.h"

FILE *entree;
FILE *sortie;
time_t start;
time_t end;
double temps;


int main(int argc, char** argv) {
    if (argc > 2) {
        if (strcmp(argv[1], "c") == 0) {
            entree = fopen(argv[2], "r");
            sortie = fopen(argv[3], "w+b");

            if (sortie == NULL || entree == NULL) {
                printf("Erreur ouverture fichier\n");
                return 0;
            }

            start = time(0);

            compression(entree, sortie);

            end = time(0);
            temps = difftime(end, start);

            fseek(entree,0,SEEK_END);
            long size_entree = (long)ftell(entree);
            fseek(entree,0,SEEK_SET);
            fseek(sortie,0,SEEK_END);
            long size_sortie = (long)ftell(sortie);
            fseek(sortie,0,SEEK_SET);
            double taux_de_compression = ((double)size_entree/(double)size_sortie);

            printf("Taux de compression: %lf\n",taux_de_compression);
            printf("Temps d'execution: %lf secondes\n",temps);

        } else {
            entree = fopen(argv[2], "rb");
            sortie = fopen(argv[3], "w");

            if (sortie == NULL || entree == NULL) {
                printf("Erreur ouverture fichier");
                 return 0;
            }

            start = time(0);
            decompression(entree, sortie);
            end = time(0);
            temps = difftime(end, start);
            printf("Temps d'execution %lf secondes\n",temps);
        }

        fclose(entree);
        fclose(sortie);
    } else {

        printf("Usage: lzw c/d <entree> <sortie>\n");

    }

    return 0;
}
