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

#include <stdbool.h>

#include "i2cusb.h"

void init(unsigned int portNr, unsigned int takt) {
	//
}

void serialDump(void) {
	//
}

char start_iic(bool MRX_ACK, unsigned short dest, char mode) {
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
	return 0;
}

void rd_bate_port(char* gelesen) {
	//
	return 0;
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
