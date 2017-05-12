/***********************************************************************
 * i2cusb.c
 * 
 * Interface fuer das USB-ITS-Geraet fuer I2C-Seminar an der TUHH.
 * 
 * Gegen diese Datei wird das Programm gelinkt, um die Schnittstelle
 * fuer das I2C-Interface zu erhalten.
 * 
 * Alle bekannten Funktionen aus der Delphi-Implementierung werden auch
 * hier implementiert.
 * 
 * Autoren: Christopher Buechse und Jan Burmeister
 * Datum: Sommersemester 2017
 **********************************************************************/

#include <stdio.h>
#include <stdbool.h>

#include "i2cusb.h"

int fd;
bool initialized = false;

void Init(int portNr, int takt) {
	char puffer[3];
	
	// Seriellen Port oeffnen
	fd = oeffne_port(fd, portNr);
	
	// Reset des USB-ITS-Geraets
	sende_befehl(fd, "XX");
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != 'X' || puffer[1] != 'X') {
		fprintf(stderr, "lese_antwort: Lesen der Antwort fehlgeschlagen! Erwartet: 'XX', bekommen '%c%c'!\n",puffer[0], puffer[1]);
		err_quit(fd);
	}
	
	// Setzen der Baudrate
	puffer[0] = 'C';
	puffer[1] = (char) takt;
	sende_befehl(fd, puffer);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != 'C' || puffer[1] != (char) takt) {
		fprintf(stderr, "lese_antwort: Lesen der Antwort fehlgeschlagen! Erwartet: 'XX', bekommen '%c%c'!\n",puffer[0], puffer[1]);
		err_quit(fd);
	}
	
	initialized = true;
}

void DeInit(void) {
	close(fd);
}

void serialDump(void) {
	//
}

char start_iic(bool MRX_ACK, short dest, char mode) {
	//
	return 0;
}

char stop_iic(void) {
	//
	return 0;
}

char wr_byte_iic(char b) {
	//
	return 0;
}

char rd_byte_iic(char* b, bool NOACK) {
	//
	return 0;
}

char restart_iic(bool MRX_ACK, char dest, char mode) {
	//
	return 0;
}

void wr_byte_port(char zuSchreiben) {
	//
}

void rd_byte_port(char* gelesen) {
	//
}


bool is_initialized(void) {
	//
	return initialized;
}

void relais_on(void) {
	//
}

void relais_off(void) {
	//
}

void led_on(void) {
	//
}

void led_off(void) {
	//
}
