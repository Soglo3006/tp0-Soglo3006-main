#include <stdlib.h>
#include <stdio.h>
#include "main.h"


typedef unsigned char byte;
typedef int error_code;

#define ERROR (-1)
#define HAS_ERROR(code) ((code) < 0)
#define HAS_NO_ERROR(code) ((code) >= 0)

/**
 * Cette fonction compare deux chaînes de caractères.       
 * @param p1 la première chaîne
 * @param p2 la deuxième chaîne
 * @return le résultat de la comparaison entre p1 et p2. Un nombre plus petit que
 * 0 dénote que la première chaîne est lexicographiquement inférieure à la deuxième.
 * Une valeur nulle indique que les deux chaînes sont égales tandis qu'une valeur positive
 * indique que la première chaîne est lexicographiquement supérieure à la deuxième.
 */
int strcmp(const char *p1, const char *p2) {
    char c1, c2;
    do {
        c1 = (char) *p1++;
        c2 = (char) *p2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
}

/**
 * Ex. 1: Calcul la longueur de la chaîne passée en paramètre selon
 * la spécification de la fonction strlen standard
 * @param s un pointeur vers le premier caractère de la chaîne
 * @return le nombre de caractères dans le code d'erreur, ou une erreur
 * si l'entrée est incorrecte
 */
error_code strlen2(const char *s) {
    if (s == NULL){
        return ERROR;
    }
    
    int taille = 0;

    while (*s!= '\0') {
        taille++;
        s++;
    }

    return taille;
}


/**
 * Ex.2 :Retourne le nombre de lignes d'un fichier sans changer la position
 * courante dans le fichier.
 * @param fp un pointeur vers le descripteur de fichier
 * @return le nombre de lignes, ou -1 si une erreur s'est produite
 */
error_code no_of_lines(FILE *fp) {
    if (fp == NULL){
        return ERROR;
    }

    int count = 0;
    int c;

    while ((c = fgetc(fp))!= EOF) {
        if (c == '\n') {
            count++;
        }
    }

    rewind(fp);
    return count;
}

/**
 * Ex.3: Lit une ligne au complet d'un fichier
 * @param fp le pointeur vers la ligne de fichier
 * @param out le pointeur vers la sortie
 * @param max_len la longueur maximale de la ligne à lire
 * @return le nombre de caractère ou ERROR si une erreur est survenue
 */
error_code readline(FILE *fp, char *out, size_t max_len) {
    if (fp == NULL || out == NULL || max_len == 0) {
        return ERROR;
    }

    int c;
    size_t i = 0;

    // Lire caractère par caractère et l'ajouter à la chaîne jusqu'à une limite ou EOF
    while (i < max_len - 1 && (c = fgetc(fp)) != EOF && c != '\n') {
        out[i++] = (char)c;
    }

    out[i] = '\0';  // Ajoute un caractère nul pour marquer la fin de la chaîne

    return i;  // Retourne le nombre de caractères lus
}

/**
 * Ex.4: Copie un bloc mémoire vers un autre
 * @param dest la destination de la copie
 * @param src  la source de la copie
 * @param len la longueur (en byte) de la source
 * @return nombre de bytes copiés ou une erreur s'il y a lieu
 */
error_code memcpy2(void *dest, const void *src, size_t len) {
    if (dest == NULL || src == NULL || len == 0) {
        return ERROR; // Vérification des entrées
    }

    byte *d = (byte *)dest;
    const byte *s = (const byte *)src;

    for (size_t i = 0; i < len; i++) {
        d[i] = s[i]; // Copie byte par byte
    }

    return (error_code)len;
}

/**
 * Ex.5: Analyse une ligne de transition
 * @param line la ligne à lire
 * @param len la longueur de la ligne
 * @return la transition ou NULL en cas d'erreur
 */

transition *parse_line(char *line, size_t len) {
    // Vérification si la ligne est valide (longueur > 2 pour être une transition)
    if (line == NULL || len < 3) {
        return NULL; // Ligne trop courte pour être une transition
    }

    // Allouer la mémoire pour la structure transition
    transition *trans = (transition *)malloc(sizeof(transition));
    if (!trans) {
        return NULL; // Erreur d'allocation de la structure
    }

    // Allouer de la mémoire pour les états
    trans->current_state = (char *)malloc(len + 1); // Plus 1 pour le caractère '\0'
    trans->next_state = (char *)malloc(len + 1);
    if (!trans->current_state || !trans->next_state) {
        // En cas d'échec d'allocation, libérer la mémoire et retourner NULL
        free(trans);
        if (trans->current_state) free(trans->current_state);
        if (trans->next_state) free(trans->next_state);
        return NULL;
    }

    // Analyse de la ligne avec sscanf pour extraire les valeurs
    int scanned = sscanf(
        line,
        "(%[^,],%c)->(%[^,],%c,%c)",
        trans->current_state,   // État actuel
        &trans->read,           // Symbole lu
        trans->next_state,      // Prochain état
        &trans->write,          // Symbole à écrire
        &trans->movement        // Mouvement (R ou L)
    );

    // Vérifier si tous les champs ont été correctement analysés
    if (scanned != 5) {
        // En cas d'erreur, libérer la mémoire et retourner NULL
        free(trans->current_state);
        free(trans->next_state);
        free(trans);
        return NULL;
    }

    return trans; // Retourner la structure de transition analysée
}

/**
 * Ex.6: Execute la machine de turing dont la description est fournie
 * @param machine_file le fichier de la description
 * @param input la chaîne d'entrée de la machine de turing
 * @return le code d'erreur
 */
error_code execute(char *machine_file, char *input) {
    return ERROR;
}

// ATTENTION! TOUT CE QUI EST ENTRE LES BALISES ༽つ۞﹏۞༼つ SERA ENLEVÉ! N'AJOUTEZ PAS D'AUTRES ༽つ۞﹏۞༼つ

// ༽つ۞﹏۞༼つ





int main() {
// ous pouvez ajouter des tests pour les fonctions ici
    //const char *test1 = "Booh Louha";
    //printf("Longueur du mot: %d\n", strlen2(test1));

    //Test 1 Fini
    //Test 2 fini
    //Test 3 fini
    //Test 4 fini

    // À faire Test 6




    return 0;
}



// ༽つ۞﹏۞༼つ