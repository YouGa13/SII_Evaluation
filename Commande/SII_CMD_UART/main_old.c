/* C library headers */
#include <stdbool.h>
#include <stdio.h> // pour les entrées/sorties
#include <errno.h> // Error integer and strerror() function
#include <unistd.h> // getopt(), write(), read(), close()
#include <stdlib.h> // bibliotheque standard
#include <sys/types.h> /* déclaration des types de base */
#include <string.h> // pour les chaines de caracteres
#include <fcntl.h> // Contains file controls like O_RDWR
#include <termios.h> // Contains POSIX terminal control definitions

bool arg_m_bool =  false;
bool arg_n_bool = false;
bool arg_b_bool = false;
bool arg_s_bool = false;

unsigned char stop = '\n';
unsigned char loop = 0;
unsigned char iteration = 1;

unsigned char timers_ms[3] ={10,50,100 };

unsigned char message[256];
unsigned int data_size = 0;
unsigned int trame_size = 0;

void funct_exit(void){
    EXIT_SUCCESS;
}

void main (int argc, char **argv) {

    
    int serial_port = open("/dev/ttyS6", O_RDWR);

    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    // Create new termios struct, we call it 'tty' for convention
    // No need for "= {0}" at the end as we'll immediately write the existing
    // config to this struct
    struct termios tty;

    // Read in existing settings, and handle any error
    // NOTE: This is important! POSIX states that the struct passed to tcsetattr()
    // must have been initialized with a call to tcgetattr() overwise behaviour
    // is undefined
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    // UART Init Registers
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON; // diseable canonical mode
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetspeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }


    int getopt_arg;
	char *optarg;


	unsigned char messageLoopFlag = 0;

	unsigned char messageNumber = 1;

	while ((getopt_arg = getopt(argc, argv, "m:bnt:sh")) != -1) {
		switch (getopt_arg) {
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
            case 'b':
                arg_b_bool = true;
				if((atoi(optarg) < 0) && (atoi(optarg)) > 1) {
                    printf("l'option b ne doit avoir comme argument que 0 ou 1");
                    funct_exit();
                }
                break;
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
            case 's': 
                arg_s_bool = true;
                stop = '\n';
                break;
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
    
    write(serial_port, trame, strlen(trame));

    close(serial_port);
    /* Free */
    free(trame);
}