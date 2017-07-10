/**
 * @file i2cusb.c
 *
 * @brief Interface für das USB-ITS-Gerät für I2C-Seminar an der TUHH.
 *
 * Gegen diese Datei wird das Programm gelinkt, um die Schnittstelle
 * für das I2C-Interface zu erhalten.
 *
 * Alle bekannten Funktionen aus der Delphi-Implementierung werden auch
 * hier implementiert.
 *
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 */

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "i2cusb.h"


//int fd;
/**
 * Diese globale Variable speichert den Zustand des USB-ITS-Geräts
 * (initialisiert oder nicht).
 **/
bool initialized = false;

// interne Funktionen
/**
 * @brief Interne Funktion zur Ausgabe des Busstatusses
 *
 * Diese Funktion schlüsselt den Busstatus nach den einzelnen Flags auf.
 *
 * @param status Statusbyte
 */
void decodeStatus(unsigned char busStatus) {
	unsigned int status = busStatus;
	printf("Busstatus: 0x%2X: ", status);
	printf("PIN: %d, ", (status&PIN ? 1 : 0));
	printf("STS: %d, ", (status&STS ? 1 : 0));
	printf("BER: %d, ", (status&BER ? 1 : 0));
	printf("AD0LRB: %d, ", (status&AD0LRB ? 1 : 0));
	printf("AAS: %d, ", (status&AAS ? 1 : 0));
	printf("LAB: %d, ", (status&LAB ? 1 : 0));
	printf("BB: %d\n\n", (status&BB ? 1 : 0));
}

// API-Funktionen
/**
 * @brief Initialisierung des USB-ITS-Geräts
 *
 * Diese Funktion initialisiert das USB-ITS-Gerät und muss zu Beginn
 * des Programms aufgerufen werden.
 *
 * @param portNr Nummer des COM- bzw. ttyUSB-Ports
 * @param takt Bustakt für I2C-Bus (SCL90, SCL45, SCL11, SCL1_5)
 *
 * @warning Anders als bei der Delphi-Implementierung ist der Bustakt zwingend anzugeben!
 */
void Init(int portNr, int takt) {

	char puffer[3];

	// Seriellen Port oeffnen
	fd = oeffne_port(fd, portNr);

	// Reset des USB-ITS-Geraets
	sende_befehl(fd, "XX");
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != 'X' || puffer[1] != 'X') {
		fprintf(stderr, "Init: Lesen der Antwort fehlgeschlagen! Erwartet: 'XX', bekommen '%c%c'!\n",puffer[0], puffer[1]);
		err_quit(fd);
	}

	// Setzen der Baudrate
	puffer[0] = 'C';
	puffer[1] = (char) takt;
	sende_befehl(fd, puffer);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != 'C' || puffer[1] != (char) takt) {
		fprintf(stderr, "Init: Lesen der Antwort fehlgeschlagen! Erwartet: 'C%c', bekommen '%c%c'!\n", (char) takt, puffer[0], puffer[1]);
		err_quit(fd);
	}

	initialized = true;
}

/**
 * @brief Freigeben der Ressourcen
 *
 * Diese Funktion terminiert den I2C-Bus und gibt den Filedeskriptor
 * frei. Idealerweise ist diese Funktion vor Beendigung des Programms
 * aufzurufen.
 *
 * @warning DeInit ist neu in der C-Implementierung hinzugekommen und
 * 			muss am Ende des Programms aufgerufen werden!
 */
void DeInit(void) {
	sende_befehl(fd, "OP"); // Stopp-Condition erzeugen

	// die Antwort ist nicht wirklich relevant...
#ifdef __WIN32
    CloseHandle(fd);
#else
	close(fd);
#endif
}

/**
 * @brief Diese Funktion ist nicht weiter von der Dokumentation spezifiziert.
 * @todo herausfinden, was diese Funktion tut
 * @warning nicht implementiert!
 */
void serialDump(void) {
	//
}

/**
 * @brief Erzeugung eines Startrahmens auf dem I2C-Bus
 *
 * @param MRX_ACK Acknowledge beim nächsten gesendeten Byte unterdrücken (true) oder nicht (false)
 * @param dest Zieladresse auf dem I2C-Bus
 * @param mode 'r' für lesenden oder 'w' für schreibenden Zugriff
 *
 * @return Status des Busses nach der Übertragung
 * @see Busstatus
 *
 * @warning Statt des Byte-Datentyps wird ein short verwendet, von dem das
 * 			niedrigere Bit verwendet wird. Es ergibt sich kein Unterschied zur
 * 			Delphi Implementierung bei der Adressierung.
 */
char start_iic(bool MRX_ACK, char dest, char mode) {

	char befehl[2];
	char puffer[2];

	// den Befehl zusammensetzen
	if(mode == 'r') {
		if(MRX_ACK == true) // mit Acknowledge
			befehl[0] = 'S';
		else
			befehl[0] = 's'; // ohne Acknowledge
	} else if(mode == 'w') {
		befehl[0] = 'T';
	} else {
		fprintf(stderr, "start_iic: Ungültiger Modus: %c\n!", mode);
		return 0;
	}

	befehl[1] = dest;

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0]) {
		fprintf(stderr, "start_iic: Lesen der Antwort fehlgeschlagen! Erwartet: '%c%c', bekommen '%c%c'!\n",
						dest, befehl[0], puffer[0], puffer[1]);
		err_quit(fd);
	}

#if DEBUG
	decodeStatus(puffer[1]);
#endif

	return puffer[1];
}

/**
 * @brief Erzeugung einer Stopp-Condition auf dem I2C-Bus
 *
 * @return Status des Busses nach Übertragung
 * @see Busstatus
 * @todo Herausfinden, warum die originale stop_iic Funktion einen Status
 * zurückgibt.
 */
char stop_iic(void) {
	char puffer[2];

	sende_befehl(fd, "OP");
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != 'O' || puffer[1] != 'P') {
		fprintf(stderr, "stop_iic: Lesen der Antwort fehlgeschlagen! Erwartet: 'OP', bekommen '%x%x'!\n",
						puffer[0] & 0xFF, puffer[1] & 0xFF);
		//err_quit(fd);
	}

	//return puffer[1];
	return 0;
}

/**
 * @brief Diese Funktion schreibt als Master ein Byte auf den I2C-Bus.
 * @param b - das zu schreibende Byte
 * @return Status des Busses nach der Übertragung
 * @see Busstatus
 */
char wr_byte_iic(char b) {

	char befehl[2];
	char puffer[2];

	befehl[0] = 'N';
	befehl[1] = b;

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0] || puffer[1] != befehl[1]) {
		fprintf(stderr, "wr_byte_iic: Lesen der Antwort fehlgeschlagen! Erwartet: '%x%x', bekommen '%x%x'!\n",
						befehl[0] & 0xFF, befehl[1] & 0xFF, puffer[0] & 0xFF, puffer[1] & 0xFF);
		//err_quit(fd);
	}

#if DEBUG
	decodeStatus(puffer[1]);
#endif

	return puffer[1];
}

/**
 * @brief Diese Funktion liest ein Byte als Master-Receiver vom I2C-Bus
 * @param b Puffer für das empfangene Byte
 * @param NOACK true: bei der nächste Übertragung wird ein negatives Acknowledge generiert
 * @return Status des Busses nach der Übertragung
 * @see Busstatus
 * @warning Auf Grund des Aufbaus des PCD8584 wird das letzte Byte ausgegeben,
 *   		was zuletzt auf dem Bus zu sehen war.
 */
char rd_byte_iic(char* b, bool NOACK) {

	char befehl[2];
	char puffer[3];

	befehl[0] = 'R';
	if(NOACK == true)
		befehl[1] = '0';
	else
		befehl[1] = '1';

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 3);
	if(puffer[0] != befehl[0]) {
		fprintf(stderr, "rd_byte_iic: Lesen der Antwort fehlgeschlagen! Erwartet: '%cxx', bekommen '%c%c%c'!\n",
						befehl[0], puffer[0], puffer[1], puffer[2]);
		err_quit(fd);
	}

	*b = puffer[1];

#if DEBUG
	decodeStatus(puffer[2]);
#endif

	return puffer[2];
}

/**
 * @brief Erzeugung eines neuen Startrahmens auf dem I2C-Bus
 * @param MRX_ACK Acknowledge beim nächsten gesendeten Byte unterdrücken (true) oder nicht (false)
 * @param dest Zieladresse auf dem I2C-Bus
 * @param mode 'r' für lesenden oder 'w' für schreibenden Zugriff
 * @return Status des Busses nach der Übertragung
 * @see Busstatus
 */
char restart_iic(bool MRX_ACK, char dest, char mode) {
	char befehl[2];
	char puffer[2];

	// den Befehl zusammensetzen
	if(mode == 'r') {
		if(MRX_ACK == true) // mit Acknowledge
			befehl[0] = 'V';
		else
			befehl[0] = 'v'; // ohne Acknowledge
	} else if(mode == 'w') {
		befehl[0] = 'U';
	} else {
		fprintf(stderr, "restart_iic: Ungültiger Modus: %c\n!", mode);
		return 0;
	}

	befehl[1] = dest;

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0]) {
		fprintf(stderr, "restart_iic: Lesen der Antwort fehlgeschlagen! Erwartet: '%cx', bekommen '%c%c'!\n",
						befehl[0], puffer[0], puffer[1]);
		err_quit(fd);
	}

#if DEBUG
	decodeStatus(puffer[1]);
#endif

	return puffer[1];
}

/**
 * @brief Schreiben eines Bytes auf den IO-Port des USB-ITS-Geräts
 * @param zuSchreiben das zu schreibende Byte
 */
void wr_byte_port(char zuSchreiben) {
	char befehl[2];
	char puffer[2];

	befehl[0] = 'W';
	befehl[1] = zuSchreiben;

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0] || puffer[1] != befehl[1]) {
		fprintf(stderr, "wr_byte_port: Lesen der Antwort fehlgeschlagen! Erwartet: '%c%c', bekommen '%c%c'!\n",
						befehl[0], befehl[1], puffer[0], puffer[1]);
		err_quit(fd);
	}
}

/**
 * @brief Lesen eines Bytes von dem IO-Port des USB-ITS-Geräts
 * @param gelesen - Puffer für das gelesene Byte
 */
void rd_byte_port(char* gelesen) {

	char befehl[2];
	char puffer[2];

	befehl[0] = 'D';
	befehl[1] = 'D';

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0]) {
		fprintf(stderr, "rd_byte_port: Lesen der Antwort fehlgeschlagen! Erwartet: '%cx', bekommen '%c%c'!\n",
						befehl[0], puffer[0], puffer[1]);
		err_quit(fd);
	}

	*gelesen = puffer[1];
}

/**
 * @brief Rückgabe des Initialisierungsstatusses
 * @return true: erfolgreiche Initialisierung
 */
bool is_initialized(void) {
	//
	return initialized;
}

/**
 * @brief Einschalten des Relais für die zusätzliche Busversorgung
 */
void relais_on(void) {

	char befehl[2];
	char puffer[2];

	befehl[0] = 'P';
	befehl[1] = '1';

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0] || puffer[1] != befehl[1]) {
		fprintf(stderr, "relais_on: Lesen der Antwort fehlgeschlagen! Erwartet: '%c%c', bekommen '%c%c'!\n",
						befehl[0], befehl[1], puffer[0], puffer[1]);
		err_quit(fd);
	}
}

/**
 * @brief Ausschalten des Relais für die zusätzliche Busversorgung
 */
void relais_off(void) {

	char befehl[2];
	char puffer[2];

	befehl[0] = 'P';
	befehl[1] = '0';

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0] || puffer[1] != befehl[1]) {
		fprintf(stderr, "relais_off: Lesen der Antwort fehlgeschlagen! Erwartet: '%c%c', bekommen '%c%c'!\n",
						befehl[0], befehl[1], puffer[0], puffer[1]);
		err_quit(fd);
	}
}

/**
 * @brief Einschalten der roten LED am USB-ITS-Gerät
 */
void led_on(void) {

	char befehl[2];
	char puffer[2];

	befehl[0] = 'L';
	befehl[1] = '1';

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0] || puffer[1] != befehl[1]) {
		fprintf(stderr, "led_on: Lesen der Antwort fehlgeschlagen! Erwartet: '%c%c', bekommen '%c%c'!\n",
						befehl[0], befehl[1], puffer[0], puffer[1]);
		err_quit(fd);
	}
}

/**
 * @brief Ausschalten der roten LED am USB-ITS-Gerät
 */
void led_off(void) {

	char befehl[2];
	char puffer[2];

	befehl[0] = 'L';
	befehl[1] = '0';

	sende_befehl(fd, befehl);
	lese_antwort(fd, puffer, 2);
	if(puffer[0] != befehl[0] || puffer[1] != befehl[1]) {
		fprintf(stderr, "led_off: Lesen der Antwort fehlgeschlagen! Erwartet: '%c%c', bekommen '%c%c'!\n",
						befehl[0], befehl[1], puffer[0], puffer[1]);
		err_quit(fd);
	}
}

/**
 * @brief Verzögerungs-Funktion, nicht Teil der offiziellen Library
 */

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

