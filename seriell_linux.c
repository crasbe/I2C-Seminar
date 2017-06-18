/**
 * @file seriell_linux.c
 * 
 * @brief Linux-Interface für serielle Schnittstelle
 * 
 * Diese Datei enthält die Funktionen, die zur Ansteuerung des
 * seriellen Interfaces des USB-ITS-Gerätes notwendig sind.
 * 
 * @warning Diese Datei ist NUR für Linux geeignet!
 * 
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 * 
 * @see http://man7.org/linux/man-pages/man3/termios.3.html
 */

#include <errno.h>   // Fehlerausgabe
#include <fcntl.h>   // Filedeskriptor einstellen
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h> // Terminal IO

#include "seriell_linux.h"

/**
 * @brief Öffnet den seriellen Port
 * 
 * Für das serielle Device wird die Termios-Struktur von Linux verwendet.
 * 
 * 
 * @param fd Filedeskriptor des seriellen Devices
 * @param port Portnummer des zu öffnenden Ports
 * @return neuer Filedeskriptor 
 */
int oeffne_port(int fd, int port) {
	
	char portname[] = PORTNAME;
	portname[sizeof(portname)/sizeof(char)-2] = (char) (port+0x30); // ITOA für Arme...
	
	// den seriellen Port öffnen.
	if((fd = open(portname, FLAGS)) < 0) {
		fprintf(stderr, "Fehler %d beim Oeffnen von %s: %s\n",  errno, portname,
					  strerror(errno));
		return -1; // Oeffnen gescheitert
	}
	
	// mögliche andere, gesetzte Flags löschen.
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
	
	/**
	 * Folgende Flags werden gesetzt:
	 * 		- C_IFLAG
	 * 			-# IXON, IXOFF, IXANY (=0):
	 * 				Software Flusskontrolle ausschalten
	 * 			-# IGNBRK (=0):
	 * 				Break-Condition ignorieren
	 * 		- C_CFLAG
	 * 			Einstellungen für 8N1
	 * 			-# PARENB (=0):
	 * 				Paritykontrolle ausschalten
	 * 			-# CSTOPB (=0):
	 * 				Ein Stopbit
	 * 			-# CSIZE (=CS8):
	 * 				auf Charaktergröße CS8 setzen
	 * 			-# CS8:
	 * 				8 bit pro Zeichen
	 * 		- C_LFLAG
	 * 			-# ICANNON (=0):
	 * 				Mit Rohdaten arbeiten 
	 * 			-# ECHO (=0):
	 * 				Empfangene Zeichen nicht wieder zurückgeben
	 * 			-# ISIG (=0):
	 * 				Steuersignale (z.B. SIGQUIT) ignorieren
	 * 		- C_OFLAG
	 * 			-# OPOST (=0):
	 * 				Mit Rohdaten arbeiten
	 * 		- C_CC: Control Characters
	 * 			-# VMIN (=0):
	 * 				Es müssen keine Zeichen gesendet werden
	 * 			-# VTIME (=10):
	 * 				Timeout auf eine Sekunde setzen
	 */
	
	seriell.c_iflag &= ~(IXON | IXOFF | IXANY);
	seriell.c_iflag &= IGNBRK;
	
	seriell.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
	seriell.c_cflag |= CS8;
	
	seriell.c_lflag &= ~(ICANON | ECHO | ISIG);
	
	seriell.c_oflag &= ~(OPOST);
	
	seriell.c_cc[VMIN] = 0;
	seriell.c_cc[VTIME] = 10;
	
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

/**
 * @brief Sendet zwei Zeichen über die serielle Schnittstelle
 * @param fd Filedeskriptor von geöffnetem seriellen Port
 * @param befehl Zeiger auf zu sendenden Befehl
 * @return Anzahl gesendeter Zeichen
 * 
 */
int sende_befehl(int fd, char* befehl) {
	int rueckgabe = write(fd, befehl, 2);

#if DEBUG
	printf("Sende Befehl: %c%c, gesendete Bytes: %d\n", befehl[0], befehl[1], rueckgabe);
#endif

	if(rueckgabe != 2) {
		fprintf(stderr, "sende_befehl: Senden des Befehls fehlgeschlagen!\n");
	}
	
	return rueckgabe;
}

/**
 * @brief Liest n Zeichen von der seriellen Schnittstelle
 * @param fd Filedeskriptor von geöffnetem seriellen Port
 * @param puffer Puffer für zu lesende Zeichen
 * @param laenge Anzahl der zu lesenden Bytes
 * @return Anzahl gelesener Bytes
 * @note Puffer MUSS mindestens drei Byte groß sein, sollte aber
 * 			>= laenge sein!
 * 
 */
int lese_antwort(int fd, char* puffer, int laenge) {
	int gelesene_bytes = read(fd, puffer, laenge);
	
#if DEBUG
	printf("Gelesene Bytes (soll/ist): %d/%d: %c%c%c\n", laenge, gelesene_bytes,
			puffer[0], puffer[1], puffer[2]);
#endif
	
	if(gelesene_bytes != laenge) {
		fprintf(stderr, "lese_antwort: Lesen der Antwort fehlgeschlagen! Bytes erwartet: %d, bekommen: %d!\n", laenge, gelesene_bytes);
	}
	
	return gelesene_bytes;
}

/**
 * @brief Terminierung des Programms
 * Diese Funktion schließt den Filedeskriptor und beendet dann das Programm
 * mit einem Fehlerwert.
 * @param fd Zu schließender Filedeskriptor
 */
void err_quit(int fd) {
	close(fd);
	exit(EXIT_FAILURE);
}
