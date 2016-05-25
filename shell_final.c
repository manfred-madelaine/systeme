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


int main(){

	char nomRepertoire[MAX_CHAR_REP];

	int i;
	while(1)
	{
		i = 0;

		invite_commande(nomRepertoire);
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
