#include "commande.h"
/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */

/* d�claration des types de base */
#include <sys/types.h>
/* pour les entr�es/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
#include <string.h>


//#include <unistd.h>


void Funct_Errors_Gestion(char argument){
	switch (argument) {
		case 'b' :
				printf("\nthe optional argument for b must be exclusively 0 or 1 \n");
				Help_Message();
				break;
		case 'n' :
				printf("\nIt is not possible to use options n and b at the same time\n");
				printf("the optional argument for n must not be less than 0 \n");
				Help_Message();
				break;
		case 'm' :
				printf("\n the total message must not exceed 256 bytes \n");
				Help_Message();
				break;
		case 's' :
				printf("\n The message must not be empty if the s option is not activated\n");
				Help_Message();
				break;
		case 't' :
		case 'h' :
		default:
				Help_Message();
	}
}

void Help_Message(void){
				printf("Utilisation du programme :\n");
                printf("-h : affiche l'aide\n");
                printf("-s : stop le programme\n");
                printf("-m \"xxx\" : message � envoyer\n");
                printf("-t \"# # #\" : configure les timers avec les temps indiqu�s\n");
                printf("-b : message affich� en morse en boucle\n");
                printf("-n # : message affich� en morse n-fois\n");
				exit(1);
}
