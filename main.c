#include <stdio.h>
#include <stdlib.h>
/* d�claration des types de base */
#include <sys/types.h>

/* pour la gestion des erreurs */
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include "commande.h"


int main(int argc, char **argv)
{

    int bool_args_n_b = 0; // contrainte n and b
    int bool_args_msg_stop = 0; // contraint m empty

    unsigned char timer1 = 10;
    unsigned char timer2 = 50;
    unsigned char timer3 = 100;

    unsigned char looping = 0;
    unsigned char nb_iteration_uc = 1;
    unsigned char msg_size_uc;

    unsigned char msg_uc[250];
    unsigned char stop_uc= (unsigned char)'\n';

    int opt;
    while ( (opt = (getopt(argc, argv, "bhm:n:t:s"))) != -1) {
        switch (opt) {
            case 'b':
					bool_args_n_b++;
                    if((atoi(optarg) > 1) || (atoi(optarg) < 0)) Funct_Errors_Gestion('b');
                    looping = (unsigned char)atoi(optarg);
                break;
            case 'n':
                    bool_args_n_b++;
                    if((bool_args_n_b == 2) || (atoi(optarg) < 0)) Funct_Errors_Gestion('n');
                    nb_iteration_uc = (unsigned char)atoi(optarg);
                break;
			case 's':
					bool_args_msg_stop++;
					if(optarg != "\n") Funct_Errors_Gestion('s');
					stop_uc = '\n';
					exit(1);
                break;
            case 'm':
                    msg_size_uc = strlen(optarg);

					strcpy(msg_uc, optarg);
                    if (msg_size_uc >= 256) Funct_Errors_Gestion('m');
					if(strcmp(msg_uc, " ") == 0){
							bool_args_msg_stop++;
					}
					if(bool_args_msg_stop == 1)	Funct_Errors_Gestion('s');
                break;
            case 't':
                timer1 = (char) atoi(strtok(optarg, " "));
                timer2 = (char) atoi(strtok(NULL, " "));
                timer3 = (char) atoi(strtok(NULL, " "));
                break;
            case 'h':
            default:
                Help_Message();
                break;
            exit(2);
        }
	}

    printf("La boucle est : %hhu\n", looping);
	printf("nbre itération est : %hhu\n", nb_iteration_uc);
	printf("Le timer1 est : %hhu\n", timer1);
	printf("Le timer2 est : %hhu\n", timer2);
	printf("Le timer3 est : %hhu\n", timer3);
	printf("La taille est : %hhu\n", msg_size_uc);
	printf("Le message est : %s\n", msg_uc);
	printf("La fin de la trame est : %hhu\n", stop_uc);
    return 0;
}
