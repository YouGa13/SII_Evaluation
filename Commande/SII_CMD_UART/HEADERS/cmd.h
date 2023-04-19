#ifndef CMD_H
#define CMD_H


/* C library headers */
#include <stdbool.h>
#include <stdio.h> // pour les entrées/sorties
#include <unistd.h> // getopt(), write(), read(), close()
#include <stdlib.h> // bibliotheque standard
#include <sys/types.h> /* déclaration des types de base */
#include <string.h> // pour les chaines de caracteres

unsigned char *trameToSend;

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


#endif