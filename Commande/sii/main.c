/* pour la gestion des erreurs */
#include <errno.h>

#define MESSAGE_FLAG_LOOP       0
#define MESSAGE_FLAG_ITERATE    1
#define MESSAGE_FLAG_STOP       2
#define MESSAGE_FLAG_LOOP       (1 << 0)
#define MESSAGE_FLAG_ITERATE    (1 << 1)
#define MESSAGE_FLAG_STOP       (1 << 2)


void main (int argc, char **argv) {
    /*********************
     *     Init UART     *
     *********************/
    int serial_port = open("/dev/ttyACM0", O_RDWR);
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
    /***********************
     *     CLI Program     *
     ***********************/
    int c;
	extern char *optarg;
	extern int optind;
	unsigned char timer1_ms = 10; 
	unsigned char timer2_ms = 50; 
	unsigned char timer3_ms = 100;
	unsigned char message[256] = "";
    unsigned char message_size = 0;
    unsigned char msg_size = 0;
	unsigned char messageLoopFlag = 0;
	unsigned char messageNumber = 1;
	while ((c = getopt(argc, argv, "t:n:bhm:s")) != -1) {
		switch (c) {
			// Help
            case 'h':
            default:
				printf("Utilisation du programme :\n");
				printf("-h : affiche l'aide\n");
				printf("-s : stop le programme\n");
				printf("-m \"xxx\" : message à envoyer\n");
				printf("-t \"# # #\" : configure les timers avec les temps indiqués\n");
				printf("-b : message affiché en morse en boucle\n");
				printf("-n # : message affiché en morse n-fois\n");
				exit(1) ;
				break;
			// Message
			case 'm':
				if(strlen(optarg) > 256) {
					printf("Message trop long : %ld caracteres (>256)\n", strlen(optarg));
					exit(2);
				} else {
					strcpy(message, optarg);
					message_size = strlen(message);
                    msg_size = strlen(message) + 7;
				}
				break;

			case 'b':
				messageLoopFlag |= (1 << MESSAGE_FLAG_LOOP);
				messageLoopFlag |= MESSAGE_FLAG_LOOP;
				break;

			case 'n':
				messageLoopFlag |= (1 << MESSAGE_FLAG_ITERATE);
				messageLoopFlag |= MESSAGE_FLAG_ITERATE;
				messageNumber = atoi(strtok(optarg, " "));
				break;

@@ -140,7 +140,7 @@ void main (int argc, char **argv) {

            // Stop
            case 's':
				messageLoopFlag |= (1 << MESSAGE_FLAG_STOP);
				messageLoopFlag |= MESSAGE_FLAG_STOP;
				break;
		}
	} 
    /* Message array */
    unsigned char *msg = (unsigned char *) malloc((7 + msg_size) * sizeof(unsigned char));
	
    /* If message is empty */
	if ((!(messageLoopFlag & 4)) && (strcmp(message, "") == 0)) {
		printf("Précisez un message '-m' ou envoyez la commande stop '-s'. Option '-h' pour l'aide.\n");
		exit(0);
	} else {
		/* Message : -m */
		printf("Message : %s\n", message);

		/* Option -b or -n */
		if ((messageLoopFlag & 1) && !(messageLoopFlag & 2)) {
		if ((messageLoopFlag & MESSAGE_FLAG_LOOP) && (!(messageLoopFlag & MESSAGE_FLAG_ITERATE)) && (!(messageLoopFlag & MESSAGE_FLAG_STOP))) {

			printf("Mode boucle : %d\n", (messageLoopFlag & 1));
            msg[0] = messageLoopFlag & 1;
			printf("Mode boucle : %d\n", (messageLoopFlag & MESSAGE_FLAG_LOOP));
            msg[0] = messageLoopFlag & MESSAGE_FLAG_LOOP;

		} else if ((messageLoopFlag & 2) && !(messageLoopFlag & 1)) {
		} else if ((messageLoopFlag & MESSAGE_FLAG_ITERATE) && (!(messageLoopFlag & MESSAGE_FLAG_LOOP)) && (!(messageLoopFlag & MESSAGE_FLAG_STOP))) {

			/* Iterations : -n */			
			printf("Nombre d'interations : %d\n", messageNumber);
            msg[1] = messageNumber;

		} else if ((!(messageLoopFlag & 1)) && (!(messageLoopFlag & 2)) && (!(messageLoopFlag & 4))) {
		} else if ((!(messageLoopFlag & MESSAGE_FLAG_LOOP)) && (!(messageLoopFlag & MESSAGE_FLAG_ITERATE)) && (!(messageLoopFlag & MESSAGE_FLAG_STOP))) {

			/* Iterations : -n */			
			printf("Nombre d'interations : %d\n", messageNumber);
            msg[1] = messageNumber;

		} else if (messageLoopFlag & 4) {
		} else if (messageLoopFlag & MESSAGE_FLAG_STOP) {

			/* Iterations : -n */			
			printf("Arret de la diffusion.\n");

            /* Options conflict (-b && -n) */
            printf("Erreur: L'option -n et -b ne doivent pas etre appelees en meme temps.\n");
		    exit(0);
        }
	}   

    if (!(messageLoopFlag & 4)) {
    if (!(messageLoopFlag & MESSAGE_FLAG_STOP)) {

        /* Setup Timers */
        msg[2] = timer1_ms;
        msg[3] = timer2_ms;
        msg[4] = timer3_ms;
        msg[5] = message_size;
        /* Iterator for copy starts after settings cases */
        int i=6;
        
        /* Paste message chars into msg at + 6 */
        for (; i < (message_size + 6); i++) {
            msg[i] = message[i-6]; // msg[6] = message[0]
            printf("Lettre i (%d) : %c\n", i, msg[i]);
        }
        /* Last char (\n) */
        msg[i] = '\n';
        printf("Lettre i (%d) fin : %d\n", i, msg[i]);
    }
    /* Write to UART*/
    printf("Sizeof msg : %d\n", msg_size);
    printf("Frame : %d %d %d %d %d %d\n", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5]);
    write(serial_port, msg, msg_size);
    /* Close serial traffic */ 
    close(serial_port);
    /* Free */
    free(msg);
}