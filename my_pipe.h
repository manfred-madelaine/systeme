/*  MY_PIPE.H
 *
 * Contient les fonctions liées à la gestion des pipes
 *
 */

#ifndef _MY_PIPE_
#define _MY_PIPE_

#include "my_shell.h"

/* Fonction: number_pipes
 * Entrees: aucune
 * 
 * Sortie: le nombre de "|" dans la ligne de commande
 * 
 * Retourne le nombre de pipes de la commande */
int number_pipes();

/* Fonction: my_pipe
 * Entrees: le nombre de "|" dans la ligne de commande
 * 
 * Sortie: -1 en cas d'erreur, 0 sinon
 * 
 * Permet d'effectuer les pipes s'ils sont présents dans la ligne de commande */
int my_pipe(int nb_pipes);

#endif
