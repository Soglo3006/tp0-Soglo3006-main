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
    if (s == NULL) {
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
    size_t i;


    for (i = 0; i < max_len - 1; i++) {
        c = fgetc(fp);
        if (c == EOF || c == '\n') {
            break;
        }
        out[i] = (char)c;
    }

    out[i] = '\0';
    return i;
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
        return ERROR;
    }

    byte *d = (byte *)dest;
    const byte *s = (const byte *)src;

    for (size_t i = 0; i < len; i++) {
        d[i] = s[i];
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
    if (line == NULL || len < 3) {
        return NULL;
    }

    transition *trans = (transition *)malloc(sizeof(transition));
    if (!trans) {
        return NULL;
    }

    trans->current_state = (char *)malloc(len + 1);
    trans->next_state = (char *)malloc(len + 1);
    if (!trans->current_state || !trans->next_state) {
        free(trans->current_state);
        free(trans->next_state);
        free(trans);
        return NULL;
    }

    int scanned = sscanf(
        line,
        "(%[^,],%c)->(%[^,],%c,%c)",
        trans->current_state,
        &trans->read,
        trans->next_state,
        &trans->write,
        &trans->movement
    );

    if (scanned != 5) {
        free(trans->current_state);
        free(trans->next_state);
        free(trans);
        return NULL;
    }

    return trans;
}


/**
 * Ex.6: Execute la machine de turing dont la description est fournie
 * @param machine_file le fichier de la description
 * @param input la chaîne d'entrée de la machine de turing
 * @return le code d'erreur
 */
error_code execute(char *machine_file, char *input) {
    FILE *fp = fopen(machine_file, "r");
    if (!fp) return ERROR;

    char initial_state[10], accept_state[10], reject_state[10];
    fscanf(fp, "%s\n%s\n%s\n", initial_state, accept_state, reject_state);


    transition *transitions[100];
    size_t transition_count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        transitions[transition_count++] = parse_line(line, strlen2(line));
    }
    fclose(fp);

    int tape_size = strlen2(input) * 2;
    char *tape = calloc(tape_size, sizeof(char));
    for (int i = 0; i < strlen2(input); i++) {
        tape[i] = input[i];
    }

    char current_state[10];
    for (int i = 0; i < 10; i++) current_state[i] = initial_state[i];
    int head_position = 0;

    while (strcmp(current_state, accept_state) != 0 && strcmp(current_state, reject_state) != 0) {
        transition *current_transition = NULL;
        for (int i = 0; i < transition_count; i++) {
            if (strcmp(transitions[i]->current_state, current_state) == 0 &&
                transitions[i]->read == tape[head_position]) {
                current_transition = transitions[i];
                break;
                }
        }
        if (!current_transition) {
            for (int i = 0; i < 10; i++) current_state[i] = reject_state[i];
            break;
        }
        tape[head_position] = current_transition->write;
        for (int i = 0; i < 10; i++) current_state[i] = current_transition->next_state[i];
        head_position += (current_transition->movement == 'R') ? 1 : -1;
    }

    int result = strcmp(current_state, accept_state) == 0 ? 1 : 0;
    free(tape);
    return result;

}




// ATTENTION! TOUT CE QUI EST ENTRE LES BALISES ༽つ۞﹏۞༼つ SERA ENLEVÉ! N'AJOUTEZ PAS D'AUTRES ༽つ۞﹏۞༼つ

// ༽つ۞﹏۞༼つ





int main(){
// Vous pouvez ajouter des tests pour les fonctions ici


    //Test 1 Fini
    //Test 2 fini
    //Test 3 fini
    //Test 4 fini

    //Test 5 fini

    //Test 6 fini


    return 0;
}



// ༽つ۞﹏۞༼つ