#include <stdio.h>
#include <stdlib.h>
/* d�claration des types de base */
#include <sys/types.h>

/* pour la gestion des erreurs */
#include <errno.h>
#include <string.h>

#include <unistd.h>

void Funct_Errors_Gestion(char argument); // Errors gestion
void Help_Message(void);


    int bool_args_n_b = 0; // contrainte n and b
    int bool_args_msg_stop = 0; // contraint m empty

    unsigned char timer1 = 10;
    unsigned char timer2 = 50;
    unsigned char timer3 = 100;

    unsigned char looping = 0;
    unsigned char nb_iteration_uc = 1;
    unsigned char msg_size_uc;

    unsigned char msg_uc[250]="";
    unsigned char stop_uc= '\n';

int main(int argc, char **argv)
{   
    
    
    char* optarg;
    int opt;
    while ( (opt = (getopt(argc, argv, "bhm:n:t:s"))) != -1) {
        switch (opt) {
            case 'b':
					bool_args_n_b++;
                    looping = (atoi(optarg));
                    printf("La boucle est : %03d\n", looping);
                break;
            case 'n':
                    bool_args_n_b++;
                    if((bool_args_n_b == 2) || (atoi(optarg) < 0)) {
                        Funct_Errors_Gestion('n');
                        exit(1);
                    };
                    nb_iteration_uc = atoi(optarg);
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

    printf("La boucle est : %03d\n", looping);
	printf("nbre itération est : %03d\n", nb_iteration_uc);
	printf("Le timer1 est : %03d\n", timer1);
	printf("Le timer2 est : %03d\n", timer2);
	printf("Le timer3 est : %03d\n", timer3);
	printf("La taille est : %03d\n", msg_size_uc);
	printf("Le message est : %s\n", msg_uc);
	printf("La fin de la trame est : %hhu\n", stop_uc);
    return 0;
}

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
				break;
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
}
