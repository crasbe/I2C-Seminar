/**
 * @file ublox.h
 *
 * @brief Ansteuerung f�r das u-blox NEO-7M GPS-Modul
 *
 * Diese Datei enth�lt alle Definitionen f�r ublox.c und wird
 * zur Ansteuerung des u-blox NEO-7M GPS-Moduls inkludiert.
 * Au�erdem wird die I2C-Adresse des Moduls hier festgelegt.
 *
 *
 * @authors Christopher B�chse und Jan Burmeister
 * @date Sommersemester 2017
 *
 * @see https://www.u-blox.com/sites/default/files/products/documents/u-blox7-V14_ReceiverDescrProtSpec_%28GPS.G7-SW-12001%29_Public.pdf
 */

#ifndef UBLOX_H_
#define UBLOX_H_

//! Die Adresse des u-blox NEO-7M Moduls betr�gt normalerweise 0x42 (66).
#define UBLOX_ADR 66  // 0x42

// Funktionsprototypen
extern int randomReadByteUblox(char adr, char* b);
extern int randomReadStreamUblox(char adr, char* b, int length);

#endif // UBLOX_H_
