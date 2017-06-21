/**
 * @file seriell_win32.h
 *
 * @brief Windows-Interface f�r serielle Schnittstelle
 *
 * Diese Datei enth�lt die Funktionen, die zur Ansteuerung des
 * seriellen Interfaces des USB-ITS-Ger�tes notwendig sind.
 *
 * @warning Diese Datei ist NUR f�r Windows geeignet!
 *
 * @authors Christopher B�chse und Jan Burmeister
 * @date Sommersemester 2017
 *
 * @see https://batchloaf.wordpress.com/2013/02/13/writing-bytes-to-a-serial-port-in-c/
 * @see https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
 */

#ifndef SERIELL_WIN_H_
#define SERIELL_WIN_H_

#include <stdio.h>
#include <windows.h>

/**
 * @brief Pr�fix des seriellen Ports
 * Das 'x' wird sp�ter durch die Portnummer ersetzt, COM ist der
 * Standard-Port unter Windows f�r eine serielle Schnittstelle (�ber
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


#endif // SERIELL_WIN_H_
