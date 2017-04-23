/***********************************************************************
 * seriell_linux.c
 * 
 * Diese Datei enthaelt die Funktionen, die zur Ansteuerung des
 * seriellen Interfaces des USB-ITS-Gerates notwendig sind.
 * Wie der Name impliziert ist diese Datei NUR fuer Linux geeignet!
 * 
 * Autoren: Christopher Buechse und Jan Burmeister
 * Datum: Sommersemester 2017
 **********************************************************************/

#include <fcntl.h>   // Filedeskriptor einstellen
#include <errno.h>   // Fehlerausgabe
#include <termios.h> // Terminal IO

#include "seriell_linux.h"

int oeffne_port(int fd, int port) {
	char portname[] = "{}{}".format(PORTNAME, port);
	
	// den seriellen Port oeffnen.
	if((fd = open(portname, FLAGS)) < 0) {
		error_message("Fehler %d beim Oeffnen von %s: %s",  errno, portname,
					  strerror(errno));
		return -1; // Oeffnen gescheitert
	}
	
	// moegliche andere, gesetzte Flags loeschen.
	fnctl(fd, F_SETFL, 0);
	
	// den seriellen Port konfigurieren mit termios
	struct termios seriell;
	
	// Attribute des Filedeskriptors auf die termios-Struktur uebertragen
	if(tcgetattr(fd, &seriell) != 0) {
		error_message("Fehler %d beim Setzen der Attribute von termios Struktur", errno);
		return -1;
	}
	
	// Baudrate fuer Ein- und Ausgabe setzen
	cfsetispeed(&seriell, BAUDRATE);
	cfsetospeed(&seriell, BAUDRATE);
	
	// IFlags:
	//  - IXON, IXOFF, IXANY: Software Flusskontrolle ausschalten
	//  - IGNBRK: Break-Condition ignorieren
	seriell.c_iflag &= ~(IXON | IXOFF | IXANY);
	seriell.c_iflag &= IGNBRK;
	
	// CFlags: 8N1
	//  - PARENB: Paritykontrolle ausschalten
	//  - CSTOPB: ein Stopbit
	//  - CSIZE: Charactergroesse setzen
	//  - CS8: 8 bit pro Zeichen
	seriell.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
	seriell.c_cflag |= CS8;
	
	// LFlags:
	//  - ICANON: mit Rohdaten arbeiten
	//  - ECHO: empfangene Zeichen nicht wieder zurueckgeben
	//  - ISIG: Steuersignale (z.B. SIGQUIT) ignorieren
	seriell.c_lflags &= ~(ICANON | ECHO | ISIG);
	
	// OFlags:
	//  - OPOST: mit Rohdaten arbeiten
	seriell.c_oflags &= ~(OPOST);
	
	// Control Characters
	//  - VMIN: minimale Anzahl an zu sendenden Zeichen
	//  - VTIME: Timeout Zeit
	seriell.cc[VMIN] = 0; // es muessen keine Zeichen gesendet werden
	seriell.cc[VTIME] = 10; // 1s 
	
	// Attribute aus der termios Struktur an Filedeskriptor uebergeben
	//  - TCSANOW: sofort uebernehmen
	if(tcsetattr(fd, TCSANOW, &seriell) != 0) {
		error_message("Fehler %d beim Schreiben der Attribute von termios Struktur", errno);
		return -1;
	}
	
	return fd;
}

int sende_befehl(int fd, char* befehl) {
#if DEBUG
	printf("Sende Befehl: %c%c", befehl[0], befehl[1]);
#endif
	return write(fd, *befehl, 2);
}
