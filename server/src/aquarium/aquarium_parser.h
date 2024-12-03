#ifndef AQUARIUM_PARSEUR_H
#define AQUARIUM_PARSEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Aquarium.h"
#include "Display.h"

#define MAX_LENGTH 1000
#define MAX_LINES 50
#define MAX_WORDS 20

struct IP_addr
{
    int a;
    int b;
    int c;
    int d;
};

/* Compte le nombre de lignes dans un fichier */
int count_lines(char *file_name);

/* Parse une ligne en un tableau de chaînes de caractères */
char **item_from_line(char str[]);

/* Parse un fichier en un tableau de chaînes de caractères */
char **to_array(char *file_name, int Nlines);

/*  Retourne len(letters) si toutes les lettres ont été trouvées chacune une fois, dans l'ordre.
    Sinon retourne 0
    Si trouve une lettre de la chaine forbidden, retourne 0.
 */
int is_in_string(char *str, char letters[], char forbidden[]);

/* Parseur principal qui retourne un pointeur vers une structure data */
int parser(char *filename, int display);

#endif /* AQUARIUM_PARSEUR_H */