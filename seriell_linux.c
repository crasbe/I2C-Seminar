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

#include <errno.h>   // Fehlerausgabe
#include <fcntl.h>   // Filedeskriptor einstellen
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h> // Terminal IO

#include "seriell_linux.h"

int oeffne_port(int fd, int port) {
	
	char portname[] = PORTNAME;
	portname[sizeof(portname)/sizeof(char)-2] = (char) (port+0x30);
	
	// den seriellen Port oeffnen.
	if((fd = open(portname, FLAGS)) < 0) {
		fprintf(stderr, "Fehler %d beim Oeffnen von %s: %s\n",  errno, portname,
					  strerror(errno));
		return -1; // Oeffnen gescheitert
	}
	
	// moegliche andere, gesetzte Flags loeschen.
	fcntl(fd, F_SETFL, 0);
	
	// den seriellen Port konfigurieren mit termios
	struct termios seriell;
	
	// Attribute des Filedeskriptors auf die termios-Struktur uebertragen
	if(tcgetattr(fd, &seriell) != 0) {
		fprintf(stderr, "Fehler %d beim Setzen der Attribute von termios Struktur\n", errno);
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
	seriell.c_lflag &= ~(ICANON | ECHO | ISIG);
	
	// OFlags:
	//  - OPOST: mit Rohdaten arbeiten
	seriell.c_oflag &= ~(OPOST);
	
	// Control Characters
	//  - VMIN: minimale Anzahl an zu sendenden Zeichen
	//  - VTIME: Timeout Zeit
	seriell.c_cc[VMIN] = 0; // es muessen keine Zeichen gesendet werden
	seriell.c_cc[VTIME] = 10; // 1s 
	
	// Attribute aus der termios Struktur an Filedeskriptor uebergeben
	//  - TCSANOW: sofort uebernehmen
	if(tcsetattr(fd, TCSANOW, &seriell) != 0) {
		fprintf(stderr, "Fehler %d beim Schreiben der Attribute von termios Struktur\n", errno);
		return -1;
	}
	
	if(fd == -1) {
		fprintf(stderr, "oeffne_port: Oeffnen von seriellem Port fehlgeschlagen!\n");
	}
	
	return fd;
}

int sende_befehl(int fd, char* befehl) {
	
	int rueckgabe = write(fd, befehl, 2);

#if DEBUG
	printf("Sende Befehl: %c%c, Rueckgabe: %d\n", befehl[0], befehl[1], rueckgabe);
#endif

	if(rueckgabe != 2) {
		fprintf(stderr, "sende_befehl: Senden des Befehls fehlgeschlagen!\n");
	}
	
	return rueckgabe;
}

int lese_antwort(int fd, char* puffer, int laenge) {
	int gelesene_bytes = read(fd, puffer, laenge);
	
#if DEBUG
	printf("Gelesene Bytes: %d (soll), %d (ist): %c%c%c\n", laenge, gelesene_bytes,
			puffer[0], puffer[1], puffer[2]);
#endif
	
	if(gelesene_bytes != laenge) {
		fprintf(stderr, "lese_antwort: Lesen der Antwort fehlgeschlagen! Bytes erwartet: %d, bekommen: %d!\n", laenge, gelesene_bytes);
	}
	
	return gelesene_bytes;
}


void err_quit(int fd) {
	close(fd);
	exit(EXIT_FAILURE);
}
