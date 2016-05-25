
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h> 

#include "my_shell.h"
#include "my_history.h"
#include "my_cat.h"
#include "my_copy.h"
#include "my_find.h"


/* Fonction: invite_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * affiche l'invité de commande
 */
void invite_commande(char* nomRepertoire)
{
	if(getcwd(nomRepertoire, MAX_CHAR_REP*sizeof(char)))
			printf("Minishell:%s> ", nomRepertoire);

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
	int done = 0; /* nombre de caractères de ligne déjà traités*/
	int arg = 0;
	while(i < strlen(ligne))
	{
		while((ligne[i] == ' ') && (ligne[i] != '\0')) /* on élimine les espaces*/
		{
			i++;
			done++;
		}
		
		i = done; /* on remet i à la bonne valeur (caractères déjà traités)*/
		
		commande[arg] = malloc(MAX_TAILLE_ARGUMENTS * sizeof(char));
		while((ligne[i] != ' ') && (ligne[i] != '\0'))
		{
			commande[arg][i-done] = ligne[i];
			i++;
		}
		
		commande[arg][i-done] = '\0';
			
	    while((ligne[i] == ' ') && (ligne[i] != '\0')) /* on élimine les espaces*/
		{
			i++;
			done++;
		}
			
		done = i;
		arg++;
	}
	
	/* une fois commande[][] initialisé, on va supprimer l'espace entre le ">" 
	 * et le nom de fichier associé le cas échéant*/
	i = 0;
	while (commande[i] != NULL)
		i++;  /* recherche du nombre d'arguments*/
	
	if (i > 1) {
		if (strcmp(commande[i-2], ">") == 0) { /* avant-dernier argument*/
			strcat(commande[i-2], commande[i-1]); /* on met le dernier argumet derriere le ">" */
			free(commande[i-1]); /* suppression du dernier élément*/
			commande[i-1] = NULL;
		}
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


/* Retourne le chemin d'une commande passée en paramètres
 * 					(chaine de caractères)
 * Retourne echec si le chemin n'est pas trouvé
 */
char* get_path(char* cmd_name) {

	/* ajout d'un "/" à la commande */
	char command_name[40];
	strcpy(command_name, "/");
	strcat(command_name, cmd_name);

	struct stat buffer[1024];
	char* cmd_path = getenv("PATH");
	char* paths[100];
	char* temp;
	int i = 0;
	int break_ = 0;

	/* strtok permet de parser le chemin de la commande suivant un délimiteur*/
	paths[i] = strtok(cmd_path, ":"); /* 1er appel en dehors de la boucle*/

	/* on free et on réalloue de la place pour temp à chaque boucle */
	temp = malloc(strlen(paths[i]) + strlen(command_name)+ 1);
	strcpy(temp, paths[i]); /* copie du chemin dans temp */
	strcat(temp, command_name); /* concaténation des deux chaînes */

	while(1){
		free(temp);
		i++;
		paths[i] = strtok(NULL, ":");
		if (paths[i] == NULL){
		/* si on arrive à la fin, il faut sortir du while */
			printf("Erreur, commande introuvable.\n");
			break_ = 1;
			break;
		}

		/* même chose qu'avant la boucle */
		temp = malloc(strlen(paths[i]) + strlen(command_name)+ 1);
		strcpy(temp, paths[i]);
		strcat(temp, command_name);

		if (stat(temp, buffer) == 0 ){
			/* on trouve un chemin avec le même nom*/
			return temp;
		}
	}
	if (break_ == 0)
		free(temp);
	return "echec";
}


/* Fonction: internal_command
 * Entrees: un entier représentant la position de la commande
 * a effectuer dans le tableau commande[]
 * 
 * Sortie: aucune
 * 
 * Execute une commande externe
 */
int internal_command(int pos) {
	if( strcmp(commande[pos], "exit") == 0)
	    {my_exit(commande); return 1;}
	    
	else if(strcmp(commande[pos], "cd") == 0)
	    {chdir(commande[pos+1]); return 1;}
	    
	else if(strcmp(commande[pos], "cat") == 0)
	    {my_cat(commande[pos+1], commande[pos+2]); return 1;}
	    
	else if(strcmp(commande[pos], "history") == 0)
	    {my_history(); return 1;}
	    
	else if(strcmp(commande[pos], "copy") == 0)
	    {my_copy(commande[pos+1], commande[pos+2]); return 1;}  
	    
	else if(strcmp(commande[pos], "find") == 0)
		{my_find(0); return 1;}
	    
	else return 0;
}




/* Fonction: executer_commande
 * Entrees: aucune
 *
 * Sortie: aucune
 *
 * Execute une commande
 */
void executer_commande() {
	
	pid_t pid;
	
	/* on commence par regarder s'il y a des pipes */
	int nb_pipes = number_pipes();
	if (nb_pipes > 0)
		my_pipe(nb_pipes);
	
	/* on exécute les commandes internes (pas de fork) */
	/* pas de pipes, donc la position de la commande est 0 */
	else if ( internal_command(0) == 0 )
	{	
		pid = fork(); /* le fork permet de lancer une commande en continuant d'executer le shell */
		if (pid == 0) /* fils */
		{
			
			/* on commence par vérifier si on veut faire une redirection */
			int i = 0; /* recherche du nombre d'éléments */
			while (commande[i] != NULL)
				i++; 
		
			if (commande[i-1][0] == '>') {
				/* ouverture d'un fichier de destination */
				int file = open(&commande[i-1][1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
						/* O_WRONLY : on va seulement écrire dans le fichier
						 * O_CREAT : créé le fichier s'il n'existe pas
						 * O_TRUNC : le fichier est tronqué à une longueur nulle s'il existe
						 * 0666 : tout le monde peut lire et écrire dans ce fichier */
				
				if (file == -1) {
					printf("Erreur, impossible de créer le fichier");
					exit(1);
				}
			
				dup2(file, 1); /* remplace la sortie standard par un fichier de sortie */
				close(file); /* fermeture du fichier de destination */
				
				/* il faut supprimer le dernier argument pour effectuer la commande externe */
				commande[i-1] = NULL;
			}
			
			/* on exécute ensuite les commandes externes */
			const char* path = get_path(commande[0]); /* on récupère le chemin */
			if (strcmp(path, "echec") != 0) { /* si on réussit à récupérer le path */
				execv(path, commande);
			}
			else
				my_exit(commande);
				
		}
		else if(pid<0)
			printf("Erreur, fork a échoué.\n");
		else /* père */
		{
			int status;
			waitpid(pid, &status, 0); /* attente bloquante */
		}
	}
	
}
