/*  MY_HISTORY.H
 *
 * Contient les fonctions liées à la sauvegarde des lignes de commande dans l'historique
 *
 */

#ifndef _HISTORY_
#define _HISTORY_

#include "my_shell.h"

/* Fonction: stockCommande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Crée un fichier "history.txt" dans lequel il enregistre les lignes de commande
 */
void stockCommande();

/* Fonction: delet
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Crée un fichier "history.txt" dans lequel il enregistre les lignes de commande
 */
void delet();

/* Fonction: my_history
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Crée un fichier "history.txt" dans lequel il enregistre les lignes de commande
 */
int my_history();

/* Fonction: my_touch
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Crée un fichier "history.txt" dans lequel il enregistre les lignes de commande
 */
int my_touch();


#endif
