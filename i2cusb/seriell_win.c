/**
 * @file seriell_win.c
 *
 * @brief Windows-Interface für serielle Schnittstelle
 *
 * Diese Datei enthält die Funktionen, die zur Ansteuerung des
 * seriellen Interfaces des USB-ITS-Gerätes notwendig sind.
 *
 * @warning Diese Datei ist NUR für Windows geeignet!
 *
 * @authors Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 *
 * @see https://batchloaf.wordpress.com/2013/02/13/writing-bytes-to-a-serial-port-in-c/
 * @see https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
 */

 #include <stdio.h>
 #include <stdbool.h>
 #include <windows.h>

 #include "seriell_win.h"

 /**
 * @brief Öffnet den seriellen Port
 *
 * Für das serielle Device wird
 *
 *
 * @param fd Filedeskriptor des seriellen Devices
 * @param port Portnummer des zu öffnenden Ports
 * @return neuer Filedeskriptor
 */
HANDLE oeffne_port(HANDLE fd, int port) {

    char portname[] = PORTNAME;
	portname[sizeof(portname)/sizeof(char)-2] = (char) (port+0x30); // ITOA für Arme...

	// open the COM port
	fd = CreateFile(portname, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(fd == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Fehler beim Oeffnen von %s\n", portname);
        CloseHandle(fd);
        //return -1;
    }

    // Auslesen der Geräteparameter
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if(GetCommState(fd, &dcbSerialParams) == 0) {
        fprintf(stderr, "Fehler beim Lesen der Parameter des seriellen Ports!\n");
        CloseHandle(fd);
        //return -1;
    }

    // Setzen/Verändern der Geräteparameter
    dcbSerialParams.BaudRate = BAUDRATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(fd, &dcbSerialParams) == 0) {
        fprintf(stderr, "Fehler beim Setzen der Parameter des seriellen Ports!\n");
        CloseHandle(fd);
        //return -1;
    }

    // Timeouts setzen
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 50;
    if(SetCommTimeouts(fd, &timeouts) == 0) {
        fprintf(stderr, "Fehler beim Setzen der Timeouts!\n");
        CloseHandle(fd);
        //return -1;
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
HANDLE sende_befehl(HANDLE fd, char* befehl) {
	DWORD gesendete_bytes = 0;

	if(WriteFile(fd, befehl, 2, &gesendete_bytes, NULL) == false) {
        fprintf(stderr, "sende_befehl: Senden des Befehls fehlgeschlagen!\n");
        CloseHandle(fd);
        //return -1;
	}

#if DEBUG
	printf("Sende Befehl: %c%c, gesendete Bytes: '%lu'\n", befehl[0], befehl[1], gesendete_bytes);
#endif

	if(gesendete_bytes != 2) {
		fprintf(stderr, "sende_befehl: Senden des Befehls fehlgeschlagen! Zu wenig Daten gesendet!\n");
        CloseHandle(fd);
        //return -1;
	}

	return fd;
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
HANDLE lese_antwort(HANDLE fd, char* puffer, int laenge) {
    DWORD gelesene_bytes = 0;

    if(ReadFile(fd, puffer, laenge, &gelesene_bytes, NULL) == FALSE) {
        fprintf(stderr, "lese_antwort: Lesen der Antwort fehlgeschlagen!\n");
        CloseHandle(fd);
        //return -1;
    }

#if DEBUG
	printf("Gelesene Bytes (soll/ist): %d/%lu: '%c%c%c'\n", laenge, gelesene_bytes,
			puffer[0], puffer[1], puffer[2]);
    fprintf(stdout, "Test!Bekommen '%x.%x'!\n",
						puffer[0] & 0xFF, puffer[1] & 0xFF);
#endif

    if(gelesene_bytes != (unsigned long) laenge) {
        fprintf(stderr, "lese_antwort: Lesen der Antwort fehlgeschlagen! Zu wenig Daten gelesen!\n");
        CloseHandle(fd);
        //return -1;
    }

    return fd;
}

/**
 * @brief Terminierung des Programms
 * Diese Funktion schließt den Filedeskriptor und beendet dann das Programm
 * mit einem Fehlerwert.
 * @param fd Zu schließender Filedeskriptor
 */
void err_quit(HANDLE fd) {
    CloseHandle(fd);
    exit(EXIT_FAILURE);
}
