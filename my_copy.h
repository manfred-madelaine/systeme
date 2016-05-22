/*  MY_COPY.H
 *
 * Contient les fonctions liées à la commande copy
 *
 */

#ifndef _MY_COPY_
#define _MY_COPY_

//#include "my_shell.h"

/*
 * Copie un fichier en conservant les mêmes stats de fichiers
 * Retourne -1 si la copie n'a pas fonctionné
 */
int copie_n(char* source, char* cible);

/*
 * Concatène deux chaines de caractères
 * retourne la concatenation 
 */
void concatener(char** res, char rep[], char fichier[]);

/*
 * Copie le statut du fichier src dans le fichier dest
 * retourne 0 si l'opération se passe bien, -1 sinon
 */
int changer_stat(char* src, char* dest);

/*
 * Copie un répertoire, en utilisant la récursivité si le dossier contient un dossier
 * retourne -1 si l'opération se passe mal
 */
int copie_dir(char source[], char destination[]);

/*
 * Prend en entrée un fchier ou un repertoire
 * Retourne 1 c'est un répertoire, 0 sinon
 */
int is_directory(char source[]);

/*
 * Copie un fichier/répertoire src dans la destination dest
 * Appel la fonction copy_dir() pour un répertoire
 * ou copy_n() pour un fichier
 */
void my_copy(char* src, char* dest);

#endif

