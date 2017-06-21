/**
 * @file seriell_win32.h
 *
 * @brief Windows-Interface (32-bit) für serielle Schnittstelle
 *
 * Diese Datei enthält die Funktionen, die zur Ansteuerung des
 * seriellen Interfaces des USB-ITS-Gerätes notwendig sind.
 *
 * @warning Diese Datei ist NUR für Windows (32-bit) geeignet!
 *
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 *
 * @see https://batchloaf.wordpress.com/2013/02/13/writing-bytes-to-a-serial-port-in-c/
 * @see https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
 */

#ifndef SERIELL_WIN32_H_
#define SERIELL_WIN32_H_

#include <stdio.h>
#include <windows.h>

/**
 * @brief Präfix des seriellen Ports
 * Das 'x' wird später durch die Portnummer ersetzt, COM ist der
 * Standard-Port unter Windows für eine serielle Schnittstelle (über
 * USB).
 */
#define PORTNAME "\\\\.\\COMx"

/**
 * @brief Baudrate des seriellen Ports
 */
#define BAUDRATE CBR_38400


// Funktionen
extern HANDLE oeffne_port(HANDLE fd, int port);
extern HANDLE sende_befehl(HANDLE fd, char* befehl);
extern HANDLE lese_antwort(HANDLE fd, char* puffer, int laenge);
extern void err_quit(HANDLE fd);


#endif // SERIELL_WIN32_H_
