#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <utime.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "my_pipe.h"

/* Fonction: number_pipes
 * Entrees: aucune
 * 
 * Sortie: le nombre de "|" dans la ligne de commande
 * 
 * Retourne le nombre de pipes de la commande */
int number_pipes()
{
	int i = 0, pipes = 0;
	while(commande[i] != NULL) {
		if (strcmp(commande[i], "|") == 0)
			pipes++;
		i++;
	}
	return pipes;
}


/* Fonction: my_pipe
 * Entrees: le nombre de "|" dans la ligne de commande
 * 
 * Sortie: -1 en cas d'erreur, 0 sinon
 * 
 * Permet d'effectuer les pipes s'ils sont présents dans la ligne de commande */
int my_pipe(int nb_pipes) {
    
    /* le nombre de commande est égale au nombre de "|" +1 */
    int nb_commands = nb_pipes + 1;
    int i = 0;

    int tab_pipes[2*nb_pipes];

    for(i = 0; i < nb_pipes; i++){
        if(pipe(tab_pipes + i*2) < 0) {
            printf("Erreur, appel à la fonction pipe impossible.\n");
            return -1;
        }
    }

    int k = 1;
    int start_of_commande[10]; /* Donne la position de départ de chaque commande dans le tableau "commande[][]" */
    start_of_commande[0] = 0; /* La première commande débute forcément à la position 0 */
    
    /* Initialisation du tableau start_of_commande[] 
     De plus, tous les pipes du tableau de commandes sont mis à NULL */
    i = 0;
    while (commande[i] != NULL) {
        if(strcmp(commande[i], "|") == 0) {
            commande[i] = NULL;
            start_of_commande[k] = i+1;
            k++;
        }
        i++;
    }

	int j = 0;
	int pid, status;
    for (i = 0; i < nb_commands; i++) {
		
		pid = fork();
		if(pid == 0) { /* fils */
			/* si ce n'est pas la dernière commande */
			if(i < nb_pipes) {
				if(dup2(tab_pipes[j + 1], 1) < 0) {
					printf("Erreur, dup2 a échoué.\n");
					return -1;
				}
			}

			/* si ce n'est pas la première commande */
			if(j != 0 ) {
				if(dup2(tab_pipes[j-2], 0) < 0) {
					printf("Erreur, dup2 a échoué.\n");
					return -1;
				}
			}

			for(k = 0; k < 2*nb_pipes; k++){
					close(tab_pipes[k]);
			}

			/* Exécution des commandes */
			if (internal_command(start_of_commande[i]) == 1) { /* commandes internes */
				my_exit(commande); /* comme nous avons utilisé fork() et que 
									la commande est interne, il faut faire exit() */
			}
			else { /* commandes externes */
				const char* path = get_path(commande[start_of_commande[i]]); /* on récupère le chemin */
				if (strcmp(path, "echec") != 0) { /* si on réussit à récupérer le path */
					execv(path, commande + start_of_commande[i]);
				}
				else {
					printf("Erreur, impossible de trouver le chemin de la commande.\n");
					my_exit(commande);
					return -1;
				}
			}
		}
		else if(pid < 0){ 
			printf("Erreur, fork a échoué.\n");
			return -1;
		}

        j += 2;
    }

    for(i = 0; i < 2 * nb_pipes; i++){
        close(tab_pipes[i]); /* fermeture des pipes */
    }

    for(i = 0; i < nb_pipes + 1; i++){
        waitpid(pid, &status, 0); /* attente bloquante */
    }
    return 0;
}


