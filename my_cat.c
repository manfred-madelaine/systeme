
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

#include "my_cat.h"
#include "my_shell.h"


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
void affiche_fichier(FILE* fichier, int option_n)
{
	int caract = 0;
	int last_car = 0;
	int l = 1;

	if (option_n == 1)
		printf("%d  ", l);
	while( (caract = fgetc(fichier)) != EOF ) /* tant que la fin n'est pas atteinte */
	{
		if (caract != '\n') {
			if (last_car == '\n') { /* en stockant le caractère précédent à afficher */
				printf("\n");		/* on peut différé l'affichage du retour chariot */
									/* cela permet de ne pas afficher celui ajouté en fin de fichier */
				if (option_n == 1)
					printf("%d  ", l);
				}

			printf("%c", caract);
		}
		else
		{
			if (last_car == '\n') {
				printf("\n");
				if (option_n == 1)
					printf("%d  ", l);
				}
			l++;
		}
		last_car = caract;
	}

	if (last_car == '\n')
		printf("\n");
}

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
void my_cat(char* fichier1, char* fichier2)
{
	if (fichier1 == NULL)
		printf("Erreur, veuillez saisir un nom de fichier pour cette commande.\n");
	else if (strcmp(fichier1, "-n") == 0)
			printf("Erreur, veuillez rentrer un nom de fichier avant l'option -n\n");
	else 
	{
		// déclaration du fichier
		FILE* f1 = NULL;
		// ouverture du fichier en mode lecture seule
		f1 = fopen(fichier1, "r");
		
		// test d'ouverture
		if (f1 == NULL)
			printf("Erreur, ouverture du premier fichier impossible.\n");
			
		else // ouverture fichier 1 ok
		{
			// on regarde si l'option -n est demandée
			int option_n = 0;
			if (fichier2 != NULL) // il faut d'abord vérifier que la chaîne existe
				if (strcmp(fichier2, "-n") == 0)
					option_n = 1;
				
			// affichage du fichier 1
			affiche_fichier(f1, option_n);
				
			if (fichier2 != NULL)
				if (strcmp(fichier2, "-n") != 0)	// cas 2 (cat FILE1 FILE2)
				{
					FILE* f2 = fopen(fichier2, "r");
					if (f2 == NULL)
						printf("Erreur, ouverture du second fichier impossible.\n");
						
					else 
						affiche_fichier(f2, 0); // 0 pour désactiver l'option -n
					
					fclose(f2);
				}	
			fclose(f1);
		}
	}
}



