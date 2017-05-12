/**
 * @file i2cusb.h
 * 
 * @brief Interface für das USB-ITS-Gerät fuer I2C-Seminar an der TUHH.
 * 
 * Diese Datei enthält alle Definitionen fuer die i2cusb.c und
 * wird zur Verwendung der Schnittstelle fuer das I2C-Interface
 * inkludiert.
 * 
 * @author Christopher Büchse und Jan Burmeister
 * @date Sommersemester 2017
 */
#ifndef I2CUSB_H_
#define I2CUSB_H_

#include <stdbool.h>

// fuer Linux, Windows und MacOSX die passende Version laden
#ifdef __linux__
#include "seriell_linux.h"
#elif defined __WIN32
#include "seriell_win32.h"
#elif defined __WIN64
#include "seriell_win64.h"
#elif defined __APPLE__ && __MACH__ // MacOS X
#include "seriell_macosx.h"
#endif

// Konstanten
#define cTimeoutInMs 1000
#define cTimeoutInit 5000
#define SCL90 'A'  /*!< SCL 90kHz PCD8584 Clocktakt */
#define SCL45 'B'  /*!< SCL 45kHz */
#define SCL11 'C'  /*!< SCL 11kHz */
#define SCL1_5 'D' /*!< SCL 1.5kHz */

/*****************************
 * Globale Variablen
 *****************************/

/*!
 * Um mit dem Delphi-Interface übereinzustimmen, ist der Filedeskriptor
 * global definiert.
 */
extern int fd;

/*****************************
 * Funktionen
 *****************************/

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
extern void Init(int portNr, int takt);

/**
 * @brief Freigeben der Ressourcen
 * 
 * Diese Funktion terminiert den I2C-Bus und gibt den Filedeskriptor
 * frei. Idealerweise ist diese Funktion vor Beendigung des Programms
 * aufzurufen.
 * 
 */
extern void DeInit(void);

/**
 * @brief Diese Funktion ist nicht weiter von der Dokumentation spezifiziert.
 * @todo herausfinden, was diese Funktion tut
 * @warning nicht implementiert!
 */
extern void serialDump(void);

/**
 * \defgroup Busstatus Busstatus-Rückgabewert für I2C-Befehle
 * @{
 * Zum Leichteren Überprüfen der Bits sind diese Masken definiert
 * 
 * \def PIN
 * 'PIN', Bit 7, (sollte nach der Übertragung 0 sein
 * 
 * \def STS
 * 'STS', Bit 5, bei externer Stop-Condition aktiv
 * 
 * \def BER
 * 'BER', Bit 4, Bus-Error: falsches Start oder Stop
 * 
 * \def AD0LRB
 * 'AD0/BRB', Bit 3, falls AAS=0: letztes empangenes Bit, also Ack-Bit
 * 
 * \def AAS
 * 'AAS', Bit 2, Adressed As Slave (als Slave-Receiver)
 * 
 * \def LAB
 * 'LAB', Bit 1, Lost arbitration
 * 
 * \def BB
 * 'BB-', Bit 0, Bus Busy, 1= Bus frei (nach Stop-Condition)
 * @}
 */
 
 /* 
 * Bit | Bez. PCD8584 | Beschreibung
 * ----+--------------+------------------------------------------
 *  7  | PIN          | (sollte nach der Übertragung 0 sein)
 *  6  |              | 0
 *  5  | STS          | bei externer Stop-Condition aktiv
 *  4  | BER          | Bus-Error: falsches Start oder Stop
 *  3  | AD0/LRB      | falls AAS=0: letztes empf. Bit, also Ack-Bit
 *  2  | AAS          | Adressed As Slave (als Slave-Receiver)
 *  1  | LAB          | Lost arbitration
 *  0  | BB-          | Bus Busy, 1=Bus frei (nach Stop-Cond.)
 * 
 * Zum leichteren Überprüfen der Bits stehen Masken zur Verfügung.
 */
#define PIN    0b10000000
#define STS    0b00100000
#define BER    0b00010000
#define AD0LRB 0b00001000
#define AAS    0b00000100
#define LAB    0b00000010
#define BB     0b00000001

/**
 * @brief Erzeugung eines Startrahmens auf dem I2C-Bus
 * 
 * @param MRX_ACK Acknowledge beim nächsten gesendeten Byte unterdrücken (true) oder nicht (false)
 * @param dest Zieladresse auf dem I2C-Bus
 * @param mode 'r' für lesenden oder 'w' für schreibenden Zugriff
 * 
 * @return Status des Busses nach der Übertragung (siehe Tabelle)
 * 
 * @warning Statt des Byte-Datentyps wird ein short verwendet, von dem das
 * 			niedrigere Bit verwendet wird. Es ergibt sich kein Unterschied zur
 * 			Delphi Implementierung bei der Adressierung.
 */
extern char start_iic(bool MRX_ACK, short dest, char mode);

/**
 * @brief Erzeugung einer Stopp-Condition auf dem I2C-Bus
 * 
 * @return Status des Busses nach Übertragung
 * @see Busstatus
 */
extern char stop_iic(void);

/**
 * wr_byte_iic:
 *   Diese Funktion schreibt als Master ein Byte auf den I2C-Bus.
 * Argumente:
 *   char b - das zu schreibende Byte
 * Rückgabewert:
 *   char - Status des Busses nach der Übertragung (siehe Tabelle)
 */
extern char wr_byte_iic(char b);

/**
 * rd_byte_iic:
 *   Diese Funktion liest ein Byte als Master-Receiver vom I2C-Bus.
 * Argumente:
 *   char* b - Puffer für das empfangene Byte
 *   bool NOACK - true: bei der nächste Übertragung wird ein negatives
 *                Acknowledge generiert
 * Rückgabewert:
 *   char - Status des Busses nach der Übertragung (siehe Tabelle)
 * Hinweis:
 *   Auf Grund des Aufbaus des PCD8584 wird das letzte Byte ausgegeben,
 *   was zuletzt auf dem Bus zu sehen war.
 */
extern char rd_byte_iic(char* b, bool NOACK);

/**
 * restart_iic:
 *   Diese Funktion erzeugt einen neuen Startrahmen auf dem I2C-Bus.
 * Argumente:
 *   siehe start_iic:
 * Rückgabewert:
 *   char - Status des Busses nach der Übertragung (siehe Tabelle)
 */
extern char restart_iic(bool MRX_ACK, char dest, char mode);

/**
 * wr_byte_port:
 *   Diese Funktion schreibt ein Byte auf den IO-Port des USB-ITS-Geräts
 * Argumente:
 *   char zuSchreiben - das zu schreibende Byte
 * Rückgabewert:
 *   void
 */
extern void wr_byte_port(char zuSchreiben);

/**
 * rd_byte_port:
 *   Diese Funktion liest ein Byte von dem IO-Port des USB-ITS-Geräts.
 * Argumente:
 *   char* gelesen - Puffer für das gelesene Byte
 * Rückgabewert:
 *   void
 */
extern void rd_byte_port(char* gelesen);

/**
 * is_initialized:
 *   Diese Funktion gibt true zurück, wenn das Gerät und der I2C-Bus
 *   erfolgreich initialisiert wurden
 * Argumente:
 *   void
 * Rückgabewert:
 *   bool - true: erfolgreiche Initialisierung
 */
extern bool is_initialized(void);

/**
 * relais_on:
 *   Diese Funktion schaltet das Relais für die zusätzliche Bus-
 *   versorgung ein.
 * Argumente:
 *   void
 * Rückgabewert:
 *   void
 */
extern void relais_on(void);

/**
 * relais_off:
 *   Diese Funktion schaltet das Relais für die zusätzliche Bus-
 *   versorgung aus.
 * Argumente:
 *   void
 * Rückgabewert:
 *   void
 */
extern void relais_off(void);

/**
 * led_on:
 *   Diese Funktion schaltet die rote LED am USB-ITS-Gerät ein.
 * Argumente:
 *   void
 * Rückgabewert:
 *   void
 */
extern void led_on(void);

/**
 * led_off:
 *   Diese Funktion schaltet die rote LED am USB-ITS-Gerät aus.
 * Argumente:
 *   void
 * Rückgabewert:
 *   void
 */
extern void led_off(void);

#endif // I2CUSB_H_
