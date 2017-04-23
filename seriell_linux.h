/***********************************************************************
 * seriell_linux.h
 * 
 * Diese Datei enthaelt alle Definitionen fuer die seriell_linux.c
 * Datei.
 * Diese Header-Datei ist NICHT zur direkten Verwendung vorgesehen,
 * sondern kann nur ueber i2cusb.c verwendet werden!
 * 
 * Autoren: Christopher Buechse und Jan Burmeister
 * Datum: Sommersemester 2017
 **********************************************************************/

#ifndef SERIELL_LINUX_H_
#define SERIELL_LINUX_H_

#include <fcntl.h>   // fuer FD Flags
#include <termios.h> // fuer Baudrate
#include <stdbool.h>

// serieller Port per USB
#define PORTNAME "/dev/ttyUSBx" // das x wird durch eine Zahl ersetzt
// die Baudrate als termios Konstante definieren
#define BAUDRATE B38400

/* Flags fuer den seriellen Port:
 *   - O_RDWR: zum Lesen und Schreiben oeffnen
 *   - O_NOCTTY: der serielle Port ist nicht das neue Kontrollterminal
 *              (weiterhin Tastatureingaben annehmen)
 *   - O_SYNC: Schreibzugriffe nicht puffern
 */
#define FLAGS (O_RDWR | O_NOCTTY | O_SYNC)


extern int oeffne_port(int port, int baudrate);

#endif // SERIELL_LINUX_H_
