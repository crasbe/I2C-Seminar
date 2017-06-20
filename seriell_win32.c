/**
 * @file seriell_linux.c
 *
 * @brief Linux-Interface f�r serielle Schnittstelle
 *
 * Diese Datei enth�lt die Funktionen, die zur Ansteuerung des
 * seriellen Interfaces des USB-ITS-Ger�tes notwendig sind.
 *
 * @warning Diese Datei ist NUR f�r Linux geeignet!
 *
 * @authors Christopher B�chse und Jan Burmeister
 * @date Sommersemester 2017
 *
 * @see https://batchloaf.wordpress.com/2013/02/13/writing-bytes-to-a-serial-port-in-c/
 * @see https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
 */

 #include <stdio.h>
 #include <windows.h>

 #include "seriell_win32.h"

 /**
 * @brief �ffnet den seriellen Port
 *
 * F�r das serielle Device wird
 *
 *
 * @param fd Filedeskriptor des seriellen Devices
 * @param port Portnummer des zu �ffnenden Ports
 * @return neuer Filedeskriptor
 */
HANDLE oeffne_port(HANDLE fd, int port) {


}

/**
 * @brief Sendet zwei Zeichen �ber die serielle Schnittstelle
 * @param fd Filedeskriptor von ge�ffnetem seriellen Port
 * @param befehl Zeiger auf zu sendenden Befehl
 * @return Anzahl gesendeter Zeichen
 *
 */
HANDLE sende_befehl(HANDLE fd, char* befehl) {

}

/**
 * @brief Liest n Zeichen von der seriellen Schnittstelle
 * @param fd Filedeskriptor von ge�ffnetem seriellen Port
 * @param puffer Puffer f�r zu lesende Zeichen
 * @param laenge Anzahl der zu lesenden Bytes
 * @return Anzahl gelesener Bytes
 * @note Puffer MUSS mindestens drei Byte gro� sein, sollte aber
 * 			>= laenge sein!
 *
 */
HANDLE lese_antwort(HANDLE fd, char* puffer, int laenge) {

}

/**
 * @brief Terminierung des Programms
 * Diese Funktion schlie�t den Filedeskriptor und beendet dann das Programm
 * mit einem Fehlerwert.
 * @param fd Zu schlie�ender Filedeskriptor
 */
void err_quit(HANDLE fd) {

}
