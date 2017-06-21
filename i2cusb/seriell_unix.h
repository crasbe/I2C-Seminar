/**
 * @file seriell_linux.h
 *
 * @brief Unix-Interface für serielle Schnittstelle
 *
 * Diese Datei enthaelt alle Definitionen für die seriell_unix.c
 * Datei.
 *
 * @warning Diese Header-Datei ist NICHT zur direkten Verwendung vorgesehen,
 * 			sondern kann nur ueber i2cusb.c verwendet werden!
 * @warning Diese Datei ist NUR für unixoide Betriebssysteme geeignet!
 * @warning Bisher wurde das Interface nur mit Linux getestet!
 *
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 */

#ifndef SERIELL_UNIX_H_
#define SERIELL_UNIX_H_

#include <fcntl.h>   // fuer FD Flags
#include <termios.h> // fuer Baudrate
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

/**
 * @brief Präfix des seriellen Ports
 *
 * Linux verwendet /dev/ttyUSBx als Device für USB zu Seriell-
 * Wandler (z.B. für den eingebauten FT232).
 * Das 'x' wird durch die entsprechende Portnummer ersetzt.
 *
 * MacOS X verwendet /dev/cu.usbserial, wobei eine Nummerierung
 * nicht vorgesehen scheint.
 *
 * @see https://pbxbook.com/other/mac-tty.html
 */
 #ifdef __linux__
#define PORTNAME "/dev/ttyUSBx"
#elifdef __APPLE__ && __MACH__
#define PORTNAME "/dev/cu.usbserial"
#endif

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
extern int oeffne_port(int fd, int port);
extern int sende_befehl(int fd, char* befehl);
extern int lese_antwort(int fd, char* puffer, int laenge);
extern void err_quit(int fd);

#endif // SERIELL_UNIX_H_
