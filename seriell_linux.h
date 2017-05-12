/**
 * @file seriell_linux.h
 * 
 * @brief Linux-Interface für serielle Schnittstelle
 * 
 * Diese Datei enthaelt alle Definitionen für die seriell_linux.c
 * Datei.
 * 
 * @warning Diese Header-Datei ist NICHT zur direkten Verwendung vorgesehen,
 * 			sondern kann nur ueber i2cusb.c verwendet werden!
 * @warning Diese Datei ist NUR für Linux geeignet!
 * 
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 */

#ifndef SERIELL_LINUX_H_
#define SERIELL_LINUX_H_

#include <fcntl.h>   // fuer FD Flags
#include <termios.h> // fuer Baudrate
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

/**
 * @brief Präfix des seriellen Ports
 * Das 'x' wird später durch die Portnummer ersetzt, ttyUSB ist der
 * Standard-Port unter Linux für eine serielle Schnittstelle über
 * USB.
 */
#define PORTNAME "/dev/ttyUSBx"

/**
 * @brief Baudrate des seriellen Ports
 * @see termios 
 */
#define BAUDRATE B38400

/**
 * @brief Flags für den seriellen Port
 * O_RDWR: zum Lesen und Schreiben oeffnen
 * O_NOCTTY: der serielle Port ist nicht das neue Kontrollterminal (weiterhin Tastatureingaben annehmen)
 * O_SYNC: Schreibzugriffe nicht puffern
 * @see termios
 */
#define FLAGS (O_RDWR | O_NOCTTY | O_SYNC)

// Funktionen
extern int oeffne_port(int port, int baudrate);
extern int sende_befehl(int fd, char* befehl);
extern int lese_antwort(int fd, char* puffer, int laenge);
extern void err_quit(int fd);

#endif // SERIELL_LINUX_H_
