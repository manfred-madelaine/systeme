/*  MY_SHELL.H
 *
 * Contient les fonctions liées à l'exécution du mini shell
 *
 */

#ifndef _MY_SHELL_
#define _MY_SHELL_

#include "my_history.h"

#define MAX_SIZE_COMMANDE 1000
#define NB_MAX_ARGUMENTS 20
#define MAX_CHAR_REP 200
#define MAX_CHAR_UTI 100
#define MAX_CHAR_MACHINE 100
#define MAX_TAILLE_ARGUMENTS 100
#define MAX_LIGNES_COMMANDE 50


char ligne[MAX_SIZE_COMMANDE]; /* contiendra la commande à exécuter*/
char* lignes_commande[MAX_LIGNES_COMMANDE]; /* contiendra toutes les lignes de commande*/
char* commande[NB_MAX_ARGUMENTS + 1];


/* Fonction: invite_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * affiche l'invité de commande
 */
void invite_commande(char* nomRepertoire, char* nomMachine, char* nomUtilisateur);

/* Fonction: parse
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * rempli le tableau d'arguments A partir de la ligne de commande
 */
void parse();


/* Fonction: lire_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Lit une commande au clavier
 */
void lire_commande();

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

/* Fonction: executer_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Execute une commande
 */
void executer_commande();


 #endif
