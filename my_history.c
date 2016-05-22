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

#include "my_history.h"

/* Fonction: stockCommande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Crée un fichier "history.txt" dans lequel il enregistre les lignes de commande
 */
void stockCommande(){
	int i=0;

	int history = open("history.txt", O_WRONLY|O_APPEND|O_CREAT,S_IWUSR|S_IRUSR);

	if(history!= -1){
		write(history,ligne,strlen(ligne));/*on copie la ligne dans l'historique*/
		write(history,"\n",1);
	}

	close(history);
}

/* Fonction: delet
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Supprimer le retour à la ligne dans une chaine de caractère
 */
void delet(){
	int i=0;

	while (i < sizeof(ligne)){
		if (ligne[i] == '\n')
			ligne[i] = ' ';
		i++;
	}
}

/* Fonction: my_history
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * plusieurs commandes possibles:
 * 		history 	:affiche l'historique
 * 		history n	:affiche les n dernières lignes de l'historique
 * 		history	!n	:exécute la nème ligne
 */
int my_history (){

	FILE* fichier = NULL;
	int i=1, sizeh=0; /* sizeh = nombres de lignes du fichier "history.c" */

	char chaine[MAX_CHAR_MACHINE] = "";

    fichier = fopen("history.txt", "r");

    /* si le fichier est vide on s'arrête*/
	if (fichier == NULL)
		{ return 1;}

	/*si on a rentré un argument et que arg!= 0 (si arg=0 on affiche tout l'historique)*/
	if(commande[1] != NULL && commande[1] != 0){

		int val;
		i=0;

		if(commande[1][0] == '!'){

			char txt[] = "";
			/* on retire le "!" */
			commande[1][0] = '0';


			/*on convertit le char en int*/
			val = atoi(commande[1]);

			i=0;
			/* on libère l'espace en mémoire*/
			while(commande[i] != NULL)
			{
				free(commande[i]);
				commande[i] = NULL;
				i++;
			}

			i=1;

			/* On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)*/
			while (fgets(ligne, MAX_CHAR_MACHINE, fichier) != NULL)
			{
				/* Une fois arrivé à la "val"-ième ligne du fichier
				 * on afficher et exécute la commande
				 */
				if(i == val){
					/*on supprime le retour à la ligne*/
					delet();

					printf("ligne de commande: %s\n", ligne);

					/*on exécute la commande et tout ce qui va avec*/
					stockCommande();
					parse();
					executer_commande();
					break;
				}
				i++;
			}
		}
		else {
			/* on converti l'argument en entier*/
			val = atoi(commande[1]);
			i=1;

			/* On compte le nombre de lignes du fichier*/
			while(fgets(chaine, MAX_CHAR_MACHINE, fichier) != NULL)
				{sizeh++;}

			fseek(fichier,0,SEEK_SET);//on se replace au début du fichier*/

			/* On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)*/
			while (fgets(chaine, MAX_CHAR_MACHINE, fichier) != NULL)
			{
				/* Une fois arrivé aux "val" dernières lignes du fichier
				 *  on commence à afficher les commandes
				 */
				if(sizeh-i < val){
					printf("%d\t%s", i, chaine);
				}
				i++;
			}
		}
	}
	/*Si il n'y a pas d'argument, on lit tout le fichier*/
	else{
		while (fgets(chaine, MAX_CHAR_MACHINE, fichier) != NULL) /* On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)*/
		{
			printf("%d\t%s", i, chaine); /* On affiche la chaîne qu'on vient de lire*/
			i++;
		}
	}

	fclose(fichier);
	return 0;
}

/* Fonction: my_touch
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * plusieurs commandes possibles:
 *		touch fichier	: crée un fichier si il n'existe pas et modifie l'heure d'accès et de modification
 * 		touch f1 ... fn	: crée les n fichiers si ils n'existent pas et modifie l'heure d'accès et de modification
 *  	touch f1 fn -m	: idem que précédement sauf qu'il modifie uniquement l'heure de modification
 */
int my_touch(){

	/* si il n'y a pas d'argument on ne peut pas travailler sur le fichier */
	if(commande[1] == NULL){
		printf("touch: fichier innexistant\n");
		return 1;
	}

	int i = 1, checkM = 0;

	/* on parcours les arguments pour créer les fichiers et regarder
	 * si l'une de ces commandes est la commande -m
	 */
	while (commande[i] != NULL){
		/*  détection de l'option "-m" */
		if( strcmp(commande[i],"-m") == 0 ){
			checkM = 1;
			i++;
			continue;
		}

		/*on crée le fichier*/
		int f = open(commande[i], O_WRONLY|O_APPEND|O_CREAT,S_IWUSR|S_IRUSR);

		if(f!= -1)
		close(f);
		i++;
	}

	i=1;
	/* si l'une des arguments est l'option -m*/
	if(checkM == 1){
		while (commande[i] != NULL){

			struct stat temp;
			struct utimbuf buf;

			/*  détection de l'option "-m" */
			if( strcmp(commande[i],"-m") == 0 ){
				i++;
				continue;
			}

			if( stat( commande[i], &temp) != 0)
				return -1;

			/* on récupère le temps courant */
			buf.modtime = time(NULL);
			buf.actime = temp.st_atime;
			utime(commande[i] , &buf);
			i++;
		}
	}
	return 0;
}
