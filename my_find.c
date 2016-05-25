#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <string.h> 
#include <unistd.h>

#include "my_find.h"

char* current_path;

void my_find(int first_call)
{
  	if(first_call == 0){ /* cas d'un premier appel à la fonction */
		free(current_path);
		current_path = NULL;
		/* récupération du répertoire de départ */
		char start_path[MAX_PATH];
		getcwd(start_path, MAX_PATH);
		
		/* on y ajoute le nom du répertoire demandé par l'utilisateur */
		current_path = malloc( (strlen(commande[1])+strlen(start_path))*sizeof(char));
		strcpy(current_path, start_path);
		strcat(current_path, "/");
		strcat(current_path,commande[1]);
	}
	
	DIR* dir = opendir(current_path);
	if (dir == NULL) {
		/* impossible d'ouvrir le répertoire, on sort de la boucle */
        printf("Erreur, impossible de trouver le répertoire demandé.\n");
		return ;
	}
	
	/* on commence par afficher le nom du répertoire */
	if (first_call == 0)
		printf("%s\n", commande[1]);
	
    while (1) {
		
        struct dirent* ent;
        if ( (ent = readdir(dir)) == NULL) {
            break;
        }

		if (ent->d_type & DT_DIR) { /* si c'est un répertoire */

			if ((strcmp (ent->d_name, "..") != 0) && (strcmp (ent->d_name, ".") != 0)) {
				
				/* ajout d'un / puis du nom du prochain fichier/repertoire */
				strcat(current_path, "/");
				strcat(current_path, ent->d_name);
				
				int i = strlen(commande[1]);
				
				/* affichage d'une ligne de la commande find */
				printf("%s/%s\n", commande[1], ent->d_name);
				
				/* on modifie commande[1] afin de pouvoir afficher des sous-répertoires */
				strcat(commande[1], "/");
				strcat(commande[1], ent->d_name);
				
				my_find(1); /* appel récursive de la fonction my_find */
				
				/* on enlève les derniers caractères ajoutés avant l'appel récursif */
				while (i < strlen(commande[1]) ) {
					commande[1][i] = '\0';
					i++;
				}
			}
		}
		else /* si c'est un simple fichier */
			printf("%s/%s\n", commande[1], ent->d_name);
			
    }
	closedir (dir);
}

