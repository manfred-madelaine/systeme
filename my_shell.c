
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

#include "my_shell.h"
#include "my_history.h"
#include "my_cat.h"

/* Fonction: invite_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * affiche l'invité de commande
 */
void invite_commande(char* nomRepertoire, char* nomMachine, char* nomUtilisateur)
{
	if(getcwd(nomRepertoire, MAX_CHAR_REP*sizeof(char)))
			printf("%s@%s:%s> ", nomUtilisateur, nomMachine, nomRepertoire);

	fflush(stdout); /* vide le buffer */
}


/* Fonction: parse
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * rempli le tableau d'arguments A partir de la ligne de commande
 */
void parse()
{
	int i = 0;
	int done = 0; /* nombre de caractères de ligne déjà traités */
	int arg = 0; /* faut-il remplir le tableau des arguments? */
	while(i < strlen(ligne))
	{
		while((ligne[i] == ' ') && (ligne[i] != '\0')) /* on élimine les espaces */
		{
			i++;
			done++;
		}

		i = done; /* on remet i à la bonne valeur (caractères déjà traités) */

		commande[arg] = malloc(MAX_TAILLE_ARGUMENTS * sizeof(char));
		while((ligne[i] != ' ') && (ligne[i] != '\0'))
		{
			if (ligne[i] == '\n'){i++;continue;}
			commande[arg][i-done] = ligne[i];
			i++;
		}

		commande[arg][i-done] = '\0';

	    while((ligne[i] == ' ') && (ligne[i] != '\0')) /* on élimine les espaces */
		{
			i++;
			done++;
		}

		done = i;
		arg++;
	}
}


/* Fonction: lire_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Lit une commande au clavier
 */
void lire_commande()
{
	fgets(ligne, MAX_SIZE_COMMANDE, stdin);
	if (strchr(ligne,'\n'))
		*strchr(ligne,'\n') = 0;

	stockCommande();

	parse(); /* on remplit la chaine commande et le tableau d'arguments */
}


/* Fonction: my_exit
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Exécute la commande "exit"
 */
void my_exit(char** buffermult)
{
	exit(0);
}

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
int my_cat(char* fichier1, char* fichier2){

	if (fichier1 == NULL){
		printf("Erreur, veuillez saisir un nom de fichier pour cette commande.\n");
		return 1;
	}

	/* ouverture du fichier en mode lecture seule*/
	FILE* f1  = fopen(fichier1, "r");

	/* test d'ouverture*/
	if (f1 == NULL){
		printf("Erreur, ouverture du premier fichier impossible.\n");
		return 1;
	}

	/* on regarde si l'option -n est demandée*/
	int option_n = 0;
	if (fichier2 != NULL){

		if (strcmp(fichier2, "-n") == 0)
			option_n = 1;

		/* affichage du fichier 1*/
		affiche_fichier(f1, option_n);

		if (option_n != 1)	/* cas 2 (cat FILE1 FILE2)*/
		{
			FILE* f2 = fopen(fichier2, "r");

			if (f2 == NULL){
				printf("Erreur, ouverture du second fichier impossible.\n");
				return 1;
			}

			affiche_fichier(f2, 0); /* 0 pour désactiver l'option -n*/

			fclose(f2);
		}
	}

	fclose(f1);
	return 0;
}

/* Fonction: executer_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Execute une commande
 */
void executer_commande(){
	pid_t pid;

	/* on exécute les commandes internes*/
	if( strcmp(commande[0], "exit") == 0)
		{my_exit(commande);}

	else if(strcmp(commande[0], "cd") == 0)
		{chdir(commande[1]);}

	else if(strcmp(commande[0], "touch") == 0)
		{my_touch();}

	else if(strcmp(commande[0], "history") == 0)
		{my_history();}

	else if(strcmp(commande[0], "cat") == 0)
		{my_cat(commande[1], commande[2]);}

	else
	{
		pid = fork();
		if (pid == 0) /* fils*/
		{
			/* exécute les commandes externes*/
            const char* path = get_path(commande[0]); /* on récupère le chemin*/
			if (strcmp(path, "echec") != 0) { /* si on réussit à récupérer le path*/
				execv(path, commande);
			}
			else
				my_exit(commande);
		}

		else if(pid<0)
			printf("Erreur, fork a échoué.\n");

		else /* père*/
		{
			int status;
			waitpid(pid, &status, 0);
		}
	}
}


int main(){

	char nomRepertoire[MAX_CHAR_REP];
	char nomUtilisateur[MAX_CHAR_UTI];
	char nomMachine[MAX_CHAR_MACHINE];

	if(getlogin_r(nomUtilisateur, MAX_CHAR_UTI*sizeof(char)) || gethostname(nomMachine, MAX_CHAR_MACHINE*sizeof(char)) == -1)
		return EXIT_FAILURE;

	*strchr(nomMachine, '.') = '\0';

	int i;
	while(1)
	{
		i = 0;

		invite_commande(nomRepertoire, nomMachine, nomUtilisateur);
		lire_commande();
		executer_commande();

		/* on libère l'espace en mémoire*/
		while(commande[i] != NULL)
		{
			free(commande[i]);
			commande[i] = NULL;
			i++;
		}
	}

	return 0;
}


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
		int history = open(commande[i], O_WRONLY|O_APPEND|O_CREAT,S_IWUSR|S_IRUSR);

		if(history!= -1)
		close(history);
		i++;
	}

	i=1;
	/* si l'une des commandes est l'option -m*/
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
