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

void Init(int portNr, int takt) {
	// Seriellen Port oeffnen
	if((fd = oeffne_port(fd, portNr)) == -1) {
		fprintf(stderr, "Oeffnen vom seriellen Port fehlgeschlagen!");
	}
	// Reset des USB-ITS-Geraets
	
}

void DeInit(void) {
	//
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
	return false;
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
