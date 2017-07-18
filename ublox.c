/**
 * @file ublox.c
 *
 * @brief Ansteuerung für das u-blox NEO-7M GPS-Modul
 *
 * Diese Datei beinhaltet Funktionen zum Lesen eines einzelnen und
 * von mehreren Bytes sowie eine Schreibfunktion zum Schreiben von
 * zwei oder mehr Bytes an das NEO-7M Modul.
 *
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 *
 * @see https://www.u-blox.com/sites/default/files/products/documents/u-blox7-V14_ReceiverDescrProtSpec_%28GPS.G7-SW-12001%29_Public.pdf
 */

#include <stdbool.h>

#include "ublox.h"
#include "i2cusb/i2cusb.h"

/**
 * @brief Interne Funktion zum Initialisieren des I2C-Busses für
 * einen Lesevorgang vom NEO-7M Modul.
 *
 * Das u-blox NEO-7M Modul erwartet folgende Startsequenz:
 *  * Startcondition (Write) mit der Adresse des Moduls erzeugen
 *  * Register auf den Bus schreiben
 *  * Neue Startcondition (Read) erzeugen
 *
 * Danach folgt der eigentliche Lesevorgang.
 *
 * @param adr die zu lesende Registeradresse
 * @return 0 bei erfolgreicher Ausführung, -1 bei Fehler
 */
int initRead(char adr) {
    // Speicher für Rückgabewert
    char rueck;

    // Startcondition auf dem Bus erzeugen, um Registeradresse zu schreiben
    rueck = start_iic(false, UBLOX_ADR, 'w');

    // kein Ack-Bit
    if(!(rueck & AD0LRB)) {
        fprintf(stderr, "initRead: Kein Acknowledge empfangen!\n");
        return -1;
    }

    // zu lesendes Register auf den Bus schreiben
    rueck = wr_byte_iic(adr);

    // kein Ack-Bit empfangen
    if(!(rueck & AD0LRB)) {
        fprintf(stderr, "initRead: Kein Acknowledge empfangen!\n");
        return -1;
    }

    // Startcondition auf dem Bus erzeugen, um Registerinhalt zu lesen
    rueck = restart_iic(false, UBLOX_ADR, 'r');

    // kein Ack-Bit
    if(!(rueck & AD0LRB)) {
        fprintf(stderr, "initRead: Kein Acknowledge empfangen!\n");
        return -1;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

/**
 * @brief Funktion zum Lesen von einem oder mehreren Bytes vom NEO-7M Modul
 *
 * Diese Funktion liest ein oder mehr Bytes aus der übergebenen
 * Registeradresse des NEO-7M Moduls.
 *
 * @param adr zu lesende Registeradresse
 * @param buffer für die zu lesenden Daten
 * @param length Anzahl der zu lesenden Bytes
 * @return 0 bei erfolgreicher Ausführung, -1 im Fehlerfall
 */
int randomReadUblox(char adr, char* buffer, unsigned int length) {
    char rueck;
    // Überprüfen, ob Anzahl der zu lesenden Bytes > 0 ist.
    if(length == 0) {
        fprintf(stderr, "randomReadUblox: Zu lesende Bytezahl muss >=1 sein!\n");
        return -1;
    }

    // I2C-Bus initialisieren
    if(initRead(adr) == -1) {
        fprintf(stderr, "randomReadUblox: Initialisierung fehlgeschlagen!\n");
        return -1;
    }

    // Dummyread, kein negatives Acknowledge generieren
    rd_byte_iic(buffer, false);

    for(int i = 0; i < length; i++) {
        // bei dem letzten Read ein negatives Acknowledge generieren
        if(length-1 == i) {
            rd_byte_iic(buffer+i, true);
        } else {
            rueck = rd_byte_iic(buffer+i , false);
            // kein Ack-Bit
            if(!(rueck & AD0LRB)) {
                fprintf(stderr, "randomReadUblox: Kein Acknowledge beim Lesen von Byte %d/%d empfangen!\n", i, length);
                return -1;
            }
        }
    }

    stop_iic();

    return 0;

}

/**
 * @brief Funktion zum Schreiben von zwei oder mehr Bytes in das NEO-7M Modul
 *
 * Diese Funktion schreibt zwei Bytes oder mehr in das NEO-7M Modul.
 * Auf einzelne Register besteht kein Schreibzugriff, es können also nur
 * UBX- oder NMEA-Befehle geschrieben werden.
 * Die Bytezahl muss mindestens zwei sein, damit das Modul den Schreibzugriff
 * vom Schreiben der Registeradresse beim Lesezugriff unterscheiden kann.
 *
 * @param b Puffer für die zu scrheibenden Daten
 * @param length Anzahl der zu schreibenden Bytes
 * @return 0 bei erfolgreicher Ausführung, -1 im Fehlerfall
 */
int writeUblox(char* b, int length) {
    // die Länge der zu schreibenden Bytes muss mindestens 2 sein
    if(length < 2) {
        fprintf(stderr, "writeUblox: Mindestlänge für Schreibzugriffe beträgt 2 Bytes!\n");
        return -1;
    }

    // Speicher für Rückgabewert
    char rueck;

    // Startcondition auf dem Bus erzeugen, um Registeradresse zu schreiben
    rueck = start_iic(true, UBLOX_ADR, 'w');

    // kein Ack-Bit
    if(!(rueck & AD0LRB)) {
        fprintf(stderr, "writeUblox: Kein Ack bei Startcondition empfangen!\n");
        return -1;
    }

    for(int i = 0; i < length; i++) {
        rueck = wr_byte_iic(*(b+i));
        if(!(rueck & AD0LRB)) {
            fprintf(stderr, "writeUblox: Kein Ack bei Schreibvorgang empfangen!\n");
            return -1;
        }
    }

    return 0;
}

