
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <fcntl.h>     

#define MAX_SIZE_COMMANDE 1000
#define NB_MAX_ARGUMENTS 20
#define MAX_CHAR_REP 200
#define MAX_CHAR_UTI 100
#define MAX_CHAR_MACHINE 100
#define MAX_TAILLE_ARGUMENTS 100
#define MAX_LIGNES_COMMANDE 50


char ligne[MAX_SIZE_COMMANDE]; // contiendra la commande à exécuter
char* lignes_commande[MAX_LIGNES_COMMANDE];// contiendra toutes les lignes de commande
char* commande[NB_MAX_ARGUMENTS + 1];
void stockCommande();

void invite_commande(char* nomRepertoire, char* nomMachine, char* nomUtilisateur)
{
	if(getcwd(nomRepertoire, MAX_CHAR_REP*sizeof(char)))
			printf("%s@%s:%s> ", nomUtilisateur, nomMachine, nomRepertoire);
}


int my_history ();

/* A partir de la ligne de commande, rempli le tableau d'arguments 
 * et la chaine de caractère de la commande
 */
void parse()
{
	int i = 0;
	int done = 0; //nombre de caractères de ligne déjà traités
	int arg = 0; // faut-il remplir le tableau des arguments?
	while(i < strlen(ligne))
	{
		while(ligne[i] == ' ') // on élimine les espaces
		{
			i++;
			done++;
		}
		
		
		// on a besoin de la taille du mot à l'indice i
		int taille = 1;
		while((ligne[i] != ' ') && (ligne[i] != '\0'))
		{
			i++; 
			taille++;
		}
		i = done; // on remet i à la bonne valeur
		
		
		commande[arg] = malloc(MAX_TAILLE_ARGUMENTS * sizeof(char));
		while((ligne[i] != ' ') && (ligne[i] != '\0'))
		{
			commande[arg][i-done] = ligne[i];
			i++;
		}
		
		commande[arg][i-done] ='\0';	
			
		done = i;
		arg++;
		
		//à enlever plus tard
		printf("comm : %s\n", commande[0]);
		printf("arg1 : %s\n", commande[1]);
		printf("arg2 : %s\n", commande[2]);
	}
}


/* Lit une commande au clavier
 * retourne -1 si la chaîne est trop longue
 */
void lire_commande()
{
	fgets(ligne, MAX_SIZE_COMMANDE, stdin);
	if (strchr(ligne,'\n')) 
		*strchr(ligne,'\n') = 0;
	
	stockCommande();
	
	parse(); // on remplit la chaine commande et le tableau d'arguments
}

/* Execute une commande
 */ 
void executer_commande()
{
	pid_t pid;
	pid = fork(); // le fork permet de lancer une commande en continuant d'executer le shell
	
	if (pid == 0) // fils
	{
				
		if(strcmp(commande[0], "cd") == 0) 
		{
			chdir(commande[1]);
		}
		
		else if(strcmp(commande[0], "history") == 0)
		{
			my_history();	// on appel notre commande interne		
		}
		
		else
		{
			execvp(commande[0], commande); // execvp(commande, table d'arguments)
		}
	}
	else if(pid<0)
		printf("Erreur, fork a échoué\n");
	else // père
	{
		int status;
		waitpid(pid, &status, 0); // attente bloquante
	}
}



int main()
{
	int i = 0;
	
	char nomRepertoire[MAX_CHAR_REP];
	char nomUtilisateur[MAX_CHAR_UTI];
	char nomMachine[MAX_CHAR_MACHINE];
		
	if(getlogin_r(nomUtilisateur, MAX_CHAR_UTI*sizeof(char)) || gethostname(nomMachine, MAX_CHAR_MACHINE*sizeof(char)) == -1)
		return EXIT_FAILURE;
	
	*strchr(nomMachine, '.') = '\0';
	
	while(1)
	{
	
		invite_commande(nomRepertoire, nomMachine, nomUtilisateur);
		lire_commande();
		executer_commande();
		
		// on libère l'espace en mémoire	
		while(commande[i] != NULL)
		{
			free(commande[i]);
			commande[i] = NULL;
			//printf("%s\n", commande[i]);
			i++;
		}
		i = 0;
	}
	
	return 0;
}


void stockCommande(){
	//on copie la ligne de commande dans l'historique
	int i=0;
	
	int history = open("history.txt", O_WRONLY|O_APPEND|O_CREAT,S_IWUSR|S_IRUSR);
	
	if(history!= -1){
		
		write(history,ligne,strlen(ligne));//on copie la ligne dans l'historique 
		write(history,"\n",1);
	}
	
	close(history);
}

/* on veut garder en mémoire toutes les lignes de commmande rentré dans le mini
 * shell. On veut donc garder toutes les lignes de commande
 * la mémoire 
 */
int my_history (){
	
	FILE* fichier = NULL;
	int i=0;
	
	char chaine[MAX_CHAR_MACHINE] = "";
	
    fichier = fopen("history.txt", "r");
    
	if (fichier != NULL){
	
		while (fgets(chaine, MAX_CHAR_MACHINE, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
            printf("%d\t%s", i, chaine); // On affiche la chaîne qu'on vient de lire
            i++;
        }
		fclose(fichier);
	}
	return 0;
}
