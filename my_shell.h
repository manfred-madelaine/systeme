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
#define MAX_CHAR_MACHINE 100
#define MAX_TAILLE_ARGUMENTS 100
#define MAX_LIGNES_COMMANDE 50
#define MAX_PATH 500


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
void invite_commande(char* nomRepertoire);

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

/* Fonction: internal_command
 * Entrees: un entier représentant la position de la commande
 * a effectuer dans le tableau commande[]
 * 
 * Sortie: aucune
 * 
 * Execute une commande externe
 */
int internal_command(int pos);

/* Fonction: executer_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Execute une commande
 */
void executer_commande();

/* Fonction: get_path
 * Entrees: le nom d'une commande
 *
 * Sortie: "echec" si lle chamin n'a pas pu être trouvé sinon, retourne une chaine de caractère
 *
 * retourne le chemin d'une commande passée en paramètres
 */
char* get_path(char* cmd_name);


 #endif
