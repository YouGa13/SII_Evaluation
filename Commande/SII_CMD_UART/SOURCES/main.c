#include "../SII_CMD_UART/HEADERS/main.h"

int main(int argc, char **argv){
    int getopt_arg;
	char *optarg;
    while ((getopt_arg = getopt(argc, argv, "m:bnt:sh")) != -1) {
		switch (getopt_arg) {
            // message
            case 'm':
                arg_m_bool = true;
				if(strlen(optarg) > 256) {
					printf("Message trop long : %d caracteres (>256)\n", strlen(optarg));
					funct_exit();
				} else {
					strcpy(message, optarg);
                    if(strcmp(message, "") == 0){
                        if(arg_s_bool == false){
                            printf("le message ne doit pas être vide lorsque l’option « s » n’est pas activé");
                            funct_exit();
                        }
                        printf("Précisez un message '-m' ou envoyez la commande stop '-s'. Option '-h' pour l'aide.\n"); 
                    }; 
                    data_size = strlen(optarg);
				}
				break;
                // boucle
            case 'b':
                arg_b_bool = true;
				if((atoi(optarg) < 0) && (atoi(optarg)) > 1) {
                    printf("l'option b ne doit avoir comme argument que 0 ou 1");
                    funct_exit();
                }
                break;
                // iteration
            case 'n':
                arg_n_bool = true;
                if(((atoi(optarg)) > 255)){
                    printf("Le nbre d'itération ne doit pas depasser 255");
                    funct_exit();
                }
                iteration = atoi(optarg);
                if(iteration > 1){
                    loop = 1;
                } else {
                    iteration = 1;
                }
                break;
                // timers
            case 't':
                    char *separators = " ";
                    int i = 0;
                    char * strToken = strtok (optarg, separators);
                    while ( (strToken != NULL ) && (i< 3)) {
                        timers_ms[i] = (unsigned char)(atoi(strToken));
                        // On demande le token suivant.
                        strToken = strtok( NULL, separators );
                        i++;
                    }
				break;
                // stop 
            case 's': 
                arg_s_bool = true;
                stop = '\n';
                break;
                // others
            case 'h':
            default:
				printf("Utilisation du programme :\n");
                printf("-h : affiche l'aide\n");
                printf("-s : stop le programme\n");
                printf("-m \"xxx\" : message à envoyer\n");
                printf("-t \"# # #\" : configure les timers avec les temps indiqués\n");
                printf("-b : message affiché en morse en boucle\n");
                printf("-n # : message affiché en morse n-fois\n");
				funct_exit();
				break;
		}
	} 

    unsigned char *trame = (unsigned char *) malloc((7 + data_size) * sizeof(unsigned char)); // trame to send
	if((arg_m_bool == false)){
        printf("l’argument « m » est obligatoire, les autres sont optionnels.");
    };

    if((arg_b_bool == true) && (arg_n_bool == true)) {
        printf("les options « n » et « b » ne peuvent pas être utilisés en même temps");
        funct_exit();
    };
    if(arg_m_bool == true){
            trame[0] = loop; 
            printf("%d", loop);
            trame[1] = iteration;
            printf("%d", iteration);
        
        
            trame[2] = timers_ms[0];
            printf("%d", timers_ms[0]);
            trame[3] = timers_ms[1];
            printf("%d", timers_ms[1]);
            trame[4] = timers_ms[2];
            printf("%d", timers_ms[2]);
            trame[5] = data_size;
            printf("%d", data_size);
            for(int i=0; i<data_size; ++i){
                trame[5 + i] = message[i];
                printf("Lettre i (%d) : %u\n", i, trame[i]);
            }
            trame[data_size + 6] = stop; 
            printf("%c", stop);
    }
    
    
    /* Free */
    free(trame);


}

