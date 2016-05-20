/*  MY_CAT.H
 *
 * Contient les fonctions liées à la commande cat
 *
 */

#ifndef _MY_CAT_
#define _MY_CAT_

#include "my_shell.h"

/* Fonction: my_exit
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Exécute la commande "exit"
 */
void my_exit(char** buffermult);

/* Fonction: affiche_fichier
 * Entrees: fichier
 * 			option d'affichage
 *
 * Sortie: aucune
 *
 * Affiche le contenu d'un fichier
 * Si option_n vaut 1, la numérotation des lignes est activée,
 * sinon elle ne l'est pas
 * Gère le dernier retour chariot d'un fichier texte
 */
void affiche_fichier(FILE* fichier, int option_n);

/* Fonction: my_cat
 * Entrees: le premier fichier
 * 			le second fichier
 *
 * Sortie: 1 si une erreur s'est produite, 0 sinon
 *
 *
 * 2 choix possibles :
 *     - cat FILE avec comme option -n qui numérote les lignes du fichier
 *     - cat FILE1 FILE2 qui affiche la concaténation des deux fichiers
 * Ouvre les fichiers nécessaires et appel d'affiche_fichier
 */
int my_cat(char* fichier1, char* fichier2);

/* Fonction: get_path
 * Entrees: le nom d'une commande
 *
 * Sortie: "echec" si lle chamin n'a pas pu être trouvé sinon, retourne une chaine de caractère
 *
 * retourne le chemin d'une commande passée en paramètres
 */
char* get_path(char* cmd_name);
#endif
