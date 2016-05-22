#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "my_shell.h"


int main(){

	char nomRepertoire[MAX_CHAR_REP];
	char nomUtilisateur[MAX_CHAR_UTI];
	char nomMachine[MAX_CHAR_MACHINE];
/*
	if(getlogin_r(nomUtilisateur, MAX_CHAR_UTI*sizeof(char)) || gethostname(nomMachine, MAX_CHAR_MACHINE*sizeof(char)) == -1)
		return EXIT_FAILURE;

	*strchr(nomMachine, '.') = '\0';*/

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
