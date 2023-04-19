#ifndef UART_H
#define UART_H

/* C library headers */
#include <stdio.h> // pour les entrées/sorties
#include <errno.h> // Error integer and strerror() function
#include <string.h> // pour les chaines de caracteres
#include <fcntl.h> // Contains file controls like O_RDWR
#include <termios.h> // Contains POSIX terminal control definitions


void Funct_SendData_UART(int serial_port, unsigned char *trame );


#endif