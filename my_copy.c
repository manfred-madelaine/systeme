#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h> 
#include <string.h> 
#include <sys/stat.h>
#include <sys/types.h>

#include "my_copy.h"

#define BUFFER_SIZE 1000

/* Fonction: copie_n
 * Entrees: 2 chaines de caractère : le fichier source et le cible
 * 
 * Sortie: -1 si la copie n'a pas fonctionné, 0 sinon
 * 
 * Copie un fichier en conservant les mêmes stats de fichiers
 */
int copie_n(char* source, char* cible)
{
	/* déclaration de variables test pour la lecture et l'écriture */
    int test_in = 0,test_out = 0;
	char temp[BUFFER_SIZE];
	/* déclaration du statut du fichier source */
	struct stat statSource;
    
    /* ouverture des deux fichiers */
    int original = open(source, O_RDONLY);
    if (original == -1) // on vérifie que le fichier est bien ouvert */
    {
		printf("Erreur, impossible d'ouvrir le fichier source.\n");
		return -1;
	}
    
    int target = open(cible, O_WRONLY | O_CREAT);
    if (target == -1)
    {
		printf("Erreur, impossible d'ouvrir le fichier destination.\n");
		return -1;
	}
	
	/* récupération du statut du fichier source */
	if (stat(source, &statSource) == -1)
	{
		printf("Erreur, récupération du statut du fichier source impossible.\n");
		return -1;
	}
	
	/* on remplace le statut du fichier destination par celui du fichier source */
	chmod(cible, statSource.st_mode);

	/* si il n'y a pas d'erreur, on lit le fichier source et on le recopie
	 dans le fichier de destination  */
    while( (test_in = read(original,&temp,BUFFER_SIZE))>0 )   
    {
		if (test_in == -1)
		{
			printf("Erreur, lecture du fichier source impossible.\n");
			return -1;
		}	

		test_out = write(target,&temp,(ssize_t)test_in);
		
		if (test_out == -1)
		{
			printf("Erreur, écriture dans le fichier destination impossible.\n");
			return -1;
		}	
    }
   
	/* fermeture des fichiers */
    close(original);
    close(target);

	/* on arrive ici si aucune erreur n'a été retourné
	 on retourne donc 0 */
    return 0;
}

/* Fonction concatener
 * Entrées: une chaine de caractère finale res, et deux autres chaines rep et fichier
 * 
 * Sortie: aucune 
 * Concatène rep et fichier dans res
 */
void concatener(char** res, char rep[], char fichier[])
{
	int taille = strlen(rep);
	int slash = 1;
	if (rep[taille - 1] != '/')
	{
		slash = 0;
		taille ++;
	}
	
	taille += strlen(fichier);
	
	*res = calloc(taille, sizeof(char));
	if (slash == 0)
	{
		strncat(*res,rep,strlen(rep));
		strncat(*res,"/",1);
		strncat(*res,fichier,strlen(fichier));
	}
	else
	{
		strncat(*res,rep,strlen(rep));
		strncat(*res,fichier,strlen(fichier));
	}
}

/* Fonction changer_stat
 * Entrées: le nom du répertoire source et du fichier cible
 * 
 * Sortie: 0 si l'opération s'est bien déroulé, -1 sinon
 * Copie le statut du rep source dans celui du rep cible
 */
int changer_stat(char* src, char* dest){
	
	struct stat nouveauStatut;
	stat(src,&nouveauStatut);
	if(!chmod(dest,nouveauStatut.st_mode)){
		return 0;
	}
	else{
		return -1;
	}
}

/* Fonction: copie_dir
 * Entrées: le nom du répertoire source et du fichier cible
 * 
 * Sortie: -1 s'il y a eu une erreur, 0 sinon
 * Copie un répertoire
 */
int copie_dir(char source[], char destination[])
{
	DIR* src = opendir(source);
	mkdir(destination, S_IRWXU); /* création d'un nouveau répertoire */
	DIR* dest = opendir(destination);
	
	if (src != NULL) 
	{ 
        struct dirent* ent; 
        char* tempSrc = NULL; 
        char* tempDest = NULL;
        char* pathSrc = NULL;
        char* pathDest = NULL;
        int taille;
        struct stat type;
	
        while ((ent = readdir(src)) != NULL) 
        { 
            if (ent->d_name[0]!='.')
            {	
				concatener(&pathSrc, source, ent->d_name);
				concatener(&pathDest, destination, ent->d_name);
				int i = stat(pathSrc, &type); /* Récupération du statut du fichier */
				
				if(i == 0){ /* On regarde le type de fichier */
					if(S_ISREG(type.st_mode)){ /* Si le fichier est de type "courant" */
						if((copie_n(pathSrc, pathDest)) == -1) /* On le copie vers le répertoire de destination */
						{
							printf("Erreur, la copie de fichier a échoué\n");
							return -1;
						}
					}
					if(S_ISDIR(type.st_mode)) /* Si le fichier est un répertoire */
					{	
						changer_stat(pathSrc, pathDest); 
						if (copie_dir(pathSrc, pathDest) == -1) /* La récursivité nous permet de copier le contenu d'un répertoire placé dans le répertoire source */
						{
							return -1;
						}
					}	
					free(pathSrc);
					pathSrc = NULL;
					free(pathDest);
					pathDest = NULL;
				}
				else
				{
					printf("Erreur, récupération du statut du fichier impossible\n");
					return -1;
				}
			}
        }
        return 0; 
        closedir(src); 
    } 
    else 
    {
		return -1;
	}
}

/* Fonction is_directory
 * Entrées: le nom du fichier source
 * 
 * Sortie: 0 si le fichier n'est pas un répertoire, 1 sinon
 */
int is_directory(char source[]) {
	
	DIR* src = opendir(source);
	if (src != NULL) 
	{ 
        struct stat type;
		stat(source, &type);
		if(S_ISREG(type.st_mode)){
			return 0;
		}
		else return 1;
	}
	
}

/* Fonction my_copy
 * Entrées: le nom du fichier source et du fichier cible
 * 
 * Sortie: aucune
 * Effectue une copie, que ce soit un répertoire ou un fichier courant */
void my_copy(char* src, char* dest)
{	
	if (is_directory(src) == 1) { /* si on a un répertoire */
		if (copie_dir(src, dest) == -1) {
				printf("Erreur, impossible d'effectuer la copie du répertoire.\n");
		}
	}
	else { /* sinon on a un fichier courant */
		if (copie_n(src, dest) == -1) {
				printf("Erreur, impossible d'effectuer la copie du fichier.\n");
		}
	}	
}
