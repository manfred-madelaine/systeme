/*  MY_COPY.H
 *
 * Contient les fonctions liées à la commande copy
 *
 */

#ifndef _MY_COPY_
#define _MY_COPY_


/* Fonction: copie_n
 * Entrees: 2 chaines de caractère : le fichier source et le cible
 * 
 * Sortie: -1 si la copie n'a pas fonctionné, 0 sinon
 * 
 * Copie un fichier en conservant les mêmes stats de fichiers
 */
int copie_n(char* source, char* cible);

/* Fonction concatener
 * Entrées: une chaine de caractère finale res, et deux autres chaines rep et fichier
 * 
 * Sortie: aucune 
 * Concatène rep et fichier dans res
 */
void concatener(char** res, char rep[], char fichier[]);

/* Fonction changer_stat
 * Entrées: le nom du répertoire source et du fichier cible
 * 
 * Sortie: 0 si l'opération s'est bien déroulé, -1 sinon
 * Copie le statut du rep source dans celui du rep cible
 */
int changer_stat(char* src, char* dest);

/* Fonction: copie_dir
 * Entrées: le nom du répertoire source et du fichier cible
 * 
 * Sortie: -1 s'il y a eu une erreur, 0 sinon
 * Copie un répertoire
 */
int copie_dir(char source[], char destination[]);

/* Fonction is_directory
 * Entrées: le nom du fichier source
 * 
 * Sortie: 0 si le fichier n'est pas un répertoire, 1 sinon
 */
int is_directory(char source[]);

/* Fonction my_copy
 * Entrées: le nom du fichier source et du fichier cible
 * 
 * Sortie: aucune
 * Effectue une copie, que ce soit un répertoire ou un fichier courant */
void my_copy(char* src, char* dest);

#endif

